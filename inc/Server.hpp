/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:53:13 by koweit            #+#    #+#             */
/*   Updated: 2026/06/25 22:39:46 by koweit           ###   ########.fr       */
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
#include "Channel.hpp"

class Client;

class Server
{
    public:
        Server(int port, std::string pass);
        ~Server();

        int         getPort();
        void        init();
        void        run();
        void        acceptNewClient();
        void        handleClientData(int ClientFd);
        void        disconnectClient(int ClientFd);
        
        std::string                     getPass() const;
        std::map<int, Client>&          getClients();
        std::map<std::string, Channel>& getChannels();
        
    private:
        Server();
        Server(const Server& src);
        Server& operator=(const Server& src);

        std::string                     _pass;
        int                             _serverSocket;
        int                             _port;
        std::vector<struct pollfd>      _fds;
        std::map<int, Client>           _clients;
        std::map<std::string, Channel>  _channels;
};

#endif //SERVER_HPP