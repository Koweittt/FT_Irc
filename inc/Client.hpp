/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 11:49:45 by koweit            #+#    #+#             */
/*   Updated: 2026/04/30 18:03:14 by koweit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
    public:
        Client(int fd, const std::string& ipAddr);
        ~Client();
        
    private:
        Client(); 
        Client(const Client& src);
        Client& operator=(const Client& src);

        bool        _isRegistered;
        int         _fd;
        std::string _ipAddr;
        std::string _nickname;
        std::string _username;
        std::string _buffer;
};

#endif //CLIENT_HPP