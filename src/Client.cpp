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
    this->_hasPassword = false;
    this->_nickname = "";
    this->_username = "";
    this->_password = "";
    this->_buffer = "";
}

Client::Client(const Client& src)
{
    this->_fd = src._fd;
    this->_ipAddr = src._ipAddr;
    this->_isRegistered = src._isRegistered;
    this->_hasPassword = src._hasPassword;
    this->_nickname = src._nickname;
    this->_username = src._username;
    this->_password = src._password;
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
        this->_hasPassword = oth._hasPassword;
        this->_nickname = oth._nickname;
        this->_username = oth._username;
        this->_password = oth._password;
        this->_buffer = oth._buffer;
    }
    return *this;
}

std::string&    Client::getBuffer()
{
    return this->_buffer;
}

int Client::getFd() const
{
    return this->_fd;
}

const std::string& Client::getIpAddr() const
{
    return this->_ipAddr;
}

bool Client::isRegistered() const
{
    return this->_isRegistered;
}

void Client::setRegistered(bool value)
{
    this->_isRegistered = value;
}

const std::string& Client::getNickname() const
{
    return this->_nickname;
}

void Client::setNickname(const std::string& nick)
{
    this->_nickname = nick;
}

const std::string& Client::getUsername() const
{
    return this->_username;
}

void Client::setUsername(const std::string& user)
{
    this->_username = user;
}

bool Client::hasPassword() const
{
    return this->_hasPassword;
}

bool Client::hasNickname() const
{
    return !this->_nickname.empty();
}

bool Client::hasUsername() const
{
    return !this->_username.empty();
}

void Client::setPassword(const std::string& pass)
{
    this->_password = pass;
    this->_hasPassword = !pass.empty();
}

// void            Client::clearBuffer(size_t len)
// {
//     for (int i = 0; i < len; i++)
//     {
//         this->buffer[i] = '/0';
//     }
// }