/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 11:45:34 by koweit            #+#    #+#             */
/*   Updated: 2026/04/30 17:47:14 by koweit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
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
    //cree le socket/fd
    
    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        throw std::runtime_error("Error: setsockopt failed (Adress could be already in use)");
    //permet de reutilise l'adresse si jamais le serveur doit redemarrer car par defaut il est bloque apres redemarrage
    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error: fcntl failed");
    //permet de rendre le serveur non bloquant
    
    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_port);

    //creer une structure avec l'adresse pour lier le socket a l'adresse
    
    if (bind(_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
        throw std::runtime_error("Error: Unable to bind socket");

    //permet de d'ecouter sur le port, ce socket ne peut donc pas communiquer mais permet d'ecouter les requetes
    if (listen(_serverSocket, 10) == -1)
        throw std::runtime_error("Error: Unable to listen on socket");

    std::cout << "Port is listening on port " << _port << std::endl;
}