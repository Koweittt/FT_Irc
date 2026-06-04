/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:53:13 by koweit            #+#    #+#             */
/*   Updated: 2026/06/04 05:39:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <poll.h>

#include "Client.hpp"

class Client;

class Server
{
    public:
        Server(int port, std::string pass);
        ~Server();

        int         getPort();
        std::string getPass();
        void        init();
        void        run();
        void        acceptNewClient();
        void        handleClientData(int ClientFd);
        void        disconnectClient(int ClientFd);
        
    private:
        Server();
        Server(const Server& src);
        Server& operator=(const Server& src);

        std::string                 _pass;
        int                         _serverSocket;
        int                         _port;
        std::vector<struct pollfd>  _fds;
        std::map<int, Client>       _clients;
};

#endif //SERVER_HPP