/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 02:19:22 by koweit            #+#    #+#             */
/*   Updated: 2026/06/26 00:47:56 by koweit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
//Constructeur principal + Copy constructor

Channel::Channel(std::string name) : _name(name), _topic(""), _key(""), _inviteOnly(false), _topicRestricted(false), _userLimit(0)
{}

Channel::Channel(const Channel& src) : _name(src._name), _topic(src._topic), _key(src._key), _inviteOnly(src._inviteOnly), _topicRestricted(src._topicRestricted), _userLimit(src._userLimit), _members(src._members), _inviteList(src._inviteList), _operators(src._operators)
{}

Channel::Channel() : _topic(""), _key(""), _inviteOnly(false), _topicRestricted(false), _userLimit(0)
{}

Channel::~Channel() {}

Channel& Channel::operator=(const Channel& src)
{
    if (this != &src)
    {
        this->_name = src._name;
        this->_inviteList = src._inviteList;
        this->_inviteOnly = src._inviteOnly;
        this->_topicRestricted = src._topicRestricted;
        this->_key = src._key;
        this->_userLimit = src._userLimit;
        this->_topic = src._topic;
        this->_members = src._members;
        this->_operators = src._operators;
    }
    return (*this);
}

void    Channel::setTopic(std::string topic)
{
    this->_topic = topic;
}

void    Channel::setKey(std::string key)
{
    this->_key = key;
}

void    Channel::setUserLimit(int lim)
{
    this->_userLimit = lim;
}

void    Channel::setInviteOnly(bool val)
{
    this->_inviteOnly = val;
}

void    Channel::setTopicRestricted(bool val)
{
    this->_topicRestricted = val;
}

std::string Channel::getName() const
{
    return this->_name;
}

std::string Channel::getTopic() const
{
    return this->_topic;
}

std::string Channel::getKey() const
{
    return this->_key;
}

int Channel::getUserLimit() const
{
    return this->_userLimit;
}

bool Channel::isInviteOnly() const
{
    return this->_inviteOnly;
}

bool Channel::isTopicRestricted() const
{
    return this->_topicRestricted;
}

bool Channel::isMember(int fd) const
{
    return(_members.find(fd) != _members.end());
}

int Channel::getMemberCount() const
{
    return this->_members.size();
}

void Channel::removeMember(int fd)
{
    _members.erase(fd);
}

void Channel::addMember(Client* client)
{
    _members[client->getFd()] = client;
}

void Channel::addOperator(int fd)
{
    _operators.insert(fd);
}

void Channel::removeOperator(int fd)
{
    _operators.erase(fd);
}

bool Channel::isOperator(int fd) const
{
    return (_operators.find(fd) != _operators.end());
}

void Channel::addInvite(int fd)
{
    _inviteList.insert(fd);
}

bool Channel::isInvited(int fd) const
{
    return (_inviteList.find(fd) != _inviteList.end());
}

void Channel::broadcast(std::string message, int exceptFd)
{
    std::map<int, Client*>::iterator it = _members.begin();
    while (it != _members.end())
    {
        if (it->first != exceptFd)
            send(it->first, message.c_str(), message.length(), 0);
        ++it;
    }
}

std::map<int, Client*>& Channel::getMembers()
{
    return this->_members;
}