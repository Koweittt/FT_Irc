/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abignals <abignals@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 11:45:34 by koweit            #+#    #+#             */
/*   Updated: 2026/06/29 00:13:48 by abignals         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h>
#include <csignal>

volatile sig_atomic_t g_running = 1;

static void handleSignal(int)
{
    g_running = 0;
}

Server::Server()
{
    this->_port = 0000;
    this->_pass = "12345";
}

Server::Server(const Server& src)
{
    this->_port = src._port;
    this->_pass = src._pass;
}
Server& Server::operator=(const Server& src)
{
    if (this != &src)
    {
        this->_port = src._port;
        this->_pass = src._pass;
    }
    return (*this);
}

Server::Server(int port, std::string pass) : _serverSocket(-1)
{
    this->_port = port;
    this->_pass = pass;
}

Server::~Server()
{
    if (_serverSocket != -1)
        close(_serverSocket);
}

void    Server::init()
{
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1)
        throw std::runtime_error("Error: Unable to create socket");
    
    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        throw std::runtime_error("Error: setsockopt failed (Adress could be already in use)");
    }

    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_port);
    
    if (bind(_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
        throw std::runtime_error("Error: Unable to bind socket");

    if (listen(_serverSocket, 10) == -1)
        throw std::runtime_error("Error: Unable to listen on socket");

    std::cout << "Port is listening on port " << _port << std::endl;
}

void    Server::run()
{
    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handleSignal;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    struct pollfd serverPoll;
    serverPoll.fd = _serverSocket;
    serverPoll.events = POLLIN;
    this->_fds.push_back(serverPoll);

    std::cout << "Server is running..." << std::endl;
    while (g_running)
    {
        int pollCount = poll(_fds.data(), _fds.size(), 1000);
        if (pollCount == -1)
        {
            if (g_running == 0)
                break;
            throw std::runtime_error("Error: poll() failed");
        }

        for (size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents == 0)
                continue;

            if(i == 0 && (_fds[i].revents & POLLIN))
            {
                acceptNewClient();
            }
            else if(_fds[i].revents & POLLIN)
            {
                handleClientData(_fds[i].fd);
            }
        }
    }

    std::cout << "\nServer is shutting down..." << std::endl;
    for (size_t i = 0; i < _fds.size(); i++)
        close(_fds[i].fd);
    _fds.clear();
    _clients.clear();
    _channels.clear();
    _serverSocket = -1;
}

void Server::acceptNewClient()
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientFd = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientFd == -1)
    {
        std::cerr << "Error: accept() failed";
        return;
    }
    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Error: fcntl() failed for client socket" << std::endl;
        close(clientFd);
        return;
    }

    std::string ipAddr = inet_ntoa(clientAddr.sin_addr);
    _clients.insert(std::make_pair(clientFd, Client(clientFd, ipAddr)));

    struct pollfd clientPoll;
    clientPoll.fd = clientFd;
    clientPoll.events = POLLIN;
    this->_fds.push_back(clientPoll);

    std::cout << "New client connected: " << ipAddr << std::endl;
}

void Server::handleClientData(int clientFd)
{
    char buffer[512];
    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) -1, 0);

    if (bytesRead <= 0)
    {
        disconnectClient(clientFd);
        return;
    }
    buffer[bytesRead] = '\0';

    Client& client = _clients.find(clientFd)->second;
    client.getBuffer() += buffer;

    std::string &buf = client.getBuffer();
    size_t pos;
    while((pos = buf.find("\r\n")) != std::string::npos)
    {
        std::string line = buf.substr(0, pos);
        buf.erase(0, pos + 2);
        
        command cmd(line);
        executeCommand(cmd, client, *this);
        if (_clients.find(clientFd) == _clients.end())
            return;
    }
}

void Server::disconnectClient(int clientFd)
{
    std::map<int, Client>::iterator clientIt = _clients.find(clientFd);
    if (clientIt != _clients.end())
    {
        Client& client = clientIt->second;
        std::string quitMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " QUIT :Connection closed\r\n";

        std::map<std::string, Channel>::iterator it = _channels.begin();
        while (it != _channels.end())
        {
            if (it->second.isMember(clientFd))
            {
                it->second.broadcast(quitMsg, clientFd);
                it->second.removeMember(clientFd);
                if (it->second.getMemberCount() == 0)
                {
                    _channels.erase(it++);
                    continue;
                }
            }
            ++it;
        }
    }

    close(clientFd);
    _clients.erase(clientFd);

    for (size_t i = 1; i < _fds.size(); i++)
    {
        if (_fds[i].fd == clientFd)
        {
            _fds.erase(_fds.begin() + i);
            break;
        }
    }
    std::cout << "Client disconnected (fd: " << clientFd << ")" << std::endl;
}

std::string Server::getPass() const
{
    return this->_pass;
}

std::map<int, Client>& Server::getClients()
{
    return this->_clients;
}

std::map<std::string, Channel>& Server::getChannels()
{
    return this->_channels;
}