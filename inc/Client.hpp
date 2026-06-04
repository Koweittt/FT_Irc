/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 11:49:45 by koweit            #+#    #+#             */
/*   Updated: 2026/06/04 06:02:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
    public:
        Client(int fd, const std::string& ipAddr);
        Client(const Client& src);
        ~Client();
        Client& operator=(const Client& src);

        std::string&    getBuffer();
        // void            clearBuffer(size_t len);

    private:
        Client();

        bool        _isRegistered;
        int         _fd;
        std::string _ipAddr;
        std::string _nickname;
        std::string _username;
        std::string _buffer;
};

#endif //CLIENT_HPP