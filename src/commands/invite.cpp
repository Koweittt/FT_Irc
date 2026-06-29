/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abignals <abignals@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 16:52:39 by abignals          #+#    #+#             */
/*   Updated: 2026/06/29 16:52:40 by abignals         ###   ########.fr       */
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
		std::string response = ":server 461 " + client.getNickname() + " INVITE :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	if (server.getChannels().find(cmd.getParameter(1)) == server.getChannels().end())
	{
		std::string response = ":server 403 " + client.getNickname() + " " + cmd.getParameter(1) + " :No such channel\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	Channel& channel = server.getChannels()[cmd.getParameter(1)];
	if (channel.isOperator(client.getFd()) == false)
	{
		std::string response = ":server 482 " + client.getNickname() + " " + channel.getName() + " :You're not channel operator\r\n";
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
		std::string response = ":server 401 " + client.getNickname() + " " + cmd.getParameter(0) + " :No such nick/channel\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	if (channel.isMember(targetFd) == true)
	{
		std::string response = ":server 443 " + client.getNickname() + " " + it->second.getNickname() + " " + channel.getName() + " :is already on channel\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	channel.addInvite(targetFd);
	std::string response = ":server 341 " + client.getNickname() + " " + it->second.getNickname() + " " + channel.getName() + "\r\n";
	send(client.getFd(), response.c_str(), response.length(), 0);
	response = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " INVITE " + it->second.getNickname() + " " + channel.getName() + "\r\n";
	send(targetFd, response.c_str(), response.length(), 0);
}
