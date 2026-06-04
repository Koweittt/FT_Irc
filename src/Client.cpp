/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 05:20:29 by marvin            #+#    #+#             */
/*   Updated: 2026/06/04 05:20:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client(int fd, const std::string& ipAddr)
{
    this->_fd = fd;
    this->_ipAddr = ipAddr;
    this->_isRegistered = false;
    this->_nickname = "";
    this->_username = "";
    this->_buffer = "";
}

Client::Client(const Client& src)
{
    this->_fd = src._fd;
    this->_ipAddr = src._ipAddr;
    this->_isRegistered = src._isRegistered;
    this->_nickname = src._nickname;
    this->_username = src._username;
    this->_buffer = src._buffer;
}

Client::~Client() {}

Client& Client::operator=(const Client& oth)
{
    if (this != &oth)
    {
        this->_fd = oth._fd;
        this->_ipAddr = oth._ipAddr;
        this->_isRegistered = oth._isRegistered;
        this->_nickname = oth._nickname;
        this->_username = oth._username;
        this->_buffer = oth._buffer;
    }
    return *this;
}

std::string&    Client::getBuffer()
{
    return this->_buffer;
}

// void            Client::clearBuffer(size_t len)
// {
//     for (int i = 0; i < len; i++)
//     {
//         this->buffer[i] = '/0';
//     }
// }