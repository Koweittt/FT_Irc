/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 11:50:47 by koweit            #+#    #+#             */
/*   Updated: 2026/06/24 03:33:46 by koweit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <set>
#include <sys/socket.h>

#include "Client.hpp"

class Channel
{
    public:
        Channel(std::string name);
        Channel();
        Channel(const Channel& src);
        Channel& operator=(const Channel& src);
        ~Channel();

        //Setters
        void setTopic(std::string topic);
        void setKey(std::string key);
        void setUserLimit(int lim);
        void setInviteOnly(bool val);
        void setTopicRestricted(bool val);
        
        //Getters
        std::string getName() const;
        std::string getTopic() const;
        std::string getKey() const;
        int         getUserLimit() const;
        bool        isInviteOnly() const;
        bool        isTopicRestricted() const;
        
        //Membre
        bool        isMember(int fd) const;
        int         getMemberCount() const;
        void        removeMember(int fd);
        void        addMember(Client* client);
        
        //Operator
        void addOperator(int fd);
        void removeOperator(int fd);
        bool isOperator(int fd) const;
        
        //Invite
        void addInvite(int fd);
        bool isInvited(int fd) const;

        void broadcast(std::string message, int exceptFd);

        
    private:
        std::string             _name;
        std::string             _topic;
        std::string             _key;
        bool                    _inviteOnly;
        bool                    _topicRestricted;
        int                     _userLimit;
        std::map<int, Client*>  _members;
        std::set<int>           _inviteList;
        std::set<int>           _operators;
};

#endif //CHANNEL_HPP