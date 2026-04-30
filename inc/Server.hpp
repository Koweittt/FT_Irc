/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:53:13 by koweit            #+#    #+#             */
/*   Updated: 2026/04/30 14:12:18 by koweit           ###   ########.fr       */
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

class Server
{
    public:
        Server();
        Server(const Server& src);
        Server& operator=(const Server& src);
        Server(int port, std::string pass);
        ~Server();

        int         getPort();
        std::string getPass();
        
        void        init();
        
    private:
        std::string _pass;
        int         _serverSocket;
        int         _port;
};

#endif //SERVER_HPP