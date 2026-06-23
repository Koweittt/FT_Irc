/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trambure <trambure@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 11:49:45 by koweit            #+#    #+#             */
/*   Updated: 2026/06/23 15:57:48 by trambure         ###   ########.fr       */
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

        int             getFd() const;
        const std::string& getIpAddr() const;
        std::string&    getBuffer();
        
        bool            isRegistered() const;
        void            setRegistered(bool value);
        
        const std::string& getNickname() const;
        void            setNickname(const std::string& nick);
        const std::string& getUsername() const;
        void            setUsername(const std::string& user);
        
        bool            hasPassword() const;
        bool            hasNickname() const;
        bool            hasUsername() const;
        void            setPassword(const std::string& pass);
        // void            clearBuffer(size_t len);

    private:
        Client();

        bool        _isRegistered;
        bool        _hasPassword;
        int         _fd;
        std::string _ipAddr;
        std::string _nickname;
        std::string _username;
        std::string _password;
        std::string _buffer;
};

#endif //CLIENT_HPP