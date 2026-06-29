/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/26 00:57:01 by koweit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void handleJoin(const command &cmd, Client &client, Server &server)
{
	if (cmd.getParameterCount() < 1)
	{
		std::string response = ":server 461 " + client.getNickname() + " JOIN :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	if (client.isRegistered() == false)
	{
		std::string response = ":server 451 " + client.getNickname() + " :You have not registered\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	
	std::string channelName = cmd.getParameter(0);
	Channel* channelPtr = NULL;

	if (server.getChannels().find(channelName) != server.getChannels().end())
	{
		channelPtr = &server.getChannels().find(channelName)->second;
		Channel& channel = server.getChannels().find(channelName)->second;
		if (channel.isInviteOnly())
		{
			if (channel.isInvited(client.getFd()) == false)
			{
				std::string response = ":server 473 " + client.getNickname() + " " + channelName + " :Cannot join channel (+i)\r\n";
				send(client.getFd(), response.c_str(), response.length(), 0);
				return;
			}
		}
		if (channel.getKey() != "")
		{
			if (channel.getKey() != cmd.getParameter(1))
			{
				std::string response = ":server 475 " + client.getNickname() + " " + channelName + " :Cannot join channel (+k)\r\n";
				send(client.getFd(), response.c_str(), response.length(), 0);
				return;
			}	
		}
		if (channel.getUserLimit() != 0)
		{
			if (channel.getMemberCount() >= channel.getUserLimit())
			{
				std::string response = ":server 471 " + client.getNickname() + " " + channelName + " :Cannot join channel (+l)\r\n";
				send(client.getFd(), response.c_str(), response.length(), 0);
				return;
			}
		}
		channel.addMember(&client);
		channel.broadcast(":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " JOIN " + channelName + "\r\n", client.getFd());	
	}	
	else
	{
		server.getChannels()[channelName] = Channel(channelName);
		Channel& channel = server.getChannels()[channelName];
		channel.addMember(&client);
		channel.addOperator(client.getFd());
		channelPtr = &server.getChannels()[channelName];
	}

	std::string response = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " JOIN " + channelName + "\r\n";
	send(client.getFd(), response.c_str(), response.length(), 0);
	if (channelPtr->getTopic().empty())
	{
		std::string msg331 = ":server 331 " + client.getNickname() + " " + channelName + " :No topic is set\r\n";
		send(client.getFd(), msg331.c_str(), msg331.length(), 0);
	}
	else
	{
		std::string msg332 = ":server 332 " + client.getNickname() + " " + channelName + " :" + channelPtr->getTopic() + "\r\n";
		send(client.getFd(), msg332.c_str(), msg332.length(), 0);
	}
	std::string names = "";
	std::map<int, Client*>::iterator it = channelPtr->getMembers().begin();
	while(it != channelPtr->getMembers().end())
	{
		if (channelPtr->isOperator(it->first))
		{
			names += "@";
		}
		names += it->second->getNickname() + " ";
		++it;	
	}
	std::string msg353 = ":server 353 " + client.getNickname() + " = " + channelName + " :" + names + "\r\n";
	send(client.getFd(), msg353.c_str(), msg353.length(), 0);
	std::string msg366 = ":server 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list\r\n";
	send(client.getFd(), msg366.c_str(), msg366.length(), 0);
}
