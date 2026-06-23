/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trambure <trambure@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 11:45:34 by koweit            #+#    #+#             */
/*   Updated: 2026/06/23 15:57:48 by trambure         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h> 

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

Server::Server(int port, std::string pass)
{
    this->_port = port;
    this->_pass = pass;
}

Server::~Server()
{
    if (_serverSocket != -1)
        close(_serverSocket);
}

//socket(IPV4(AF_INET) ou IPV6(AF_INET6), Protocole TCP(SOCK_STREAM) ou UDP(SOCK_DGRAM))
//Permet de transformer un fd en prise Pour communiquer en reseau, specifier le type d'IP et le protocole utilise

//setsockopt() permet de modifier les options de notre socket, en l'occurence cela lui permet de redemarrer sur le meme port

//pour l'adresse ip on peut utiliser inet_pton, dans ce cas la on a utilise uniquement INADDR_ANY qui est equivalent a 0.0.0.0 pour ouvrir a n'importe quelle IP de la machine locale 127.0.0.1 ou 192.168.x.x

//fcntl rend le serveur non bloquant, permet de renvoyer une erreur inoffensive au client et donc de poursuivre les requetes, modifie directement nos fd

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
    struct pollfd serverPoll;
    serverPoll.fd = _serverSocket;
    serverPoll.events = POLLIN;
    this->_fds.push_back(serverPoll);

    std::cout << "Server is running..." << std::endl;
    while (true)
    {
        int pollCount = poll(_fds.data(), _fds.size(), 1000);
        if (pollCount == -1)
            throw std::runtime_error("Error: poll() failed");
        
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
    }
}

void Server::disconnectClient(int clientFd)
{
    close (clientFd);
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