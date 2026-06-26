/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/26 03:10:15 by koweit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void handleInvite(const command &cmd, Client &client, Server &server)
{
	if (cmd.getParameterCount() < 2)
	{
		std::string response = "461 " + client.getNickname() + " INVITE :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	if (server.getChannels().find(cmd.getParameter(1)) == server.getChannels().end())
	{
		std::string response = "403 :Channel doesn't exist\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	
	Channel& channel = server.getChannels()[cmd.getParameter(1)];
	if (channel.isOperator(client.getFd()) == false)
	{
		std::string response = "482 :You are not operator of this channel\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	
	int targetFd = -1;
	std::map<int, Client>::iterator it = server.getClients().begin();
	while (it != server.getClients().end())
	{
		if (it->second.getNickname() == cmd.getParameter(0))
		{
			targetFd = it->first;
			break;	
		}
		++it;
	}
	if (targetFd == -1)
	{
		std::string response = "401 :User not found\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	if (channel.isMember(targetFd) == true)
	{
		std::string response = "443 :User is already a member of this channel\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	channel.addInvite(targetFd);
	std::string response = "341 " + client.getNickname() + " " + it->second.getNickname() + " " + channel.getName() + "\r\n";
	send(client.getFd(), response.c_str(), response.length(), 0);
	response = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " INVITE " + it->second.getNickname() + " " + channel.getName() + "\r\n";
	send(targetFd, response.c_str(), response.length(), 0);
}
