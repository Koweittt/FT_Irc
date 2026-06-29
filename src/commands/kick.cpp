/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abignals <abignals@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/29 00:02:17 by abignals         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void handleKick(const command &cmd, Client &client, Server &server)
{
	if (cmd.getParameterCount() < 2)
	{
		std::string response = ":server 461 " + client.getNickname() + " KICK :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	if (server.getChannels().find(cmd.getParameter(0)) == server.getChannels().end())
	{
		std::string response = ":server 403 " + client.getNickname() + " " + cmd.getParameter(0) + " :No such channel\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	Channel& channel = server.getChannels()[cmd.getParameter(0)];
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
		if (it->second.getNickname() == cmd.getParameter(1))
		{
			targetFd = it->first;
			break;
		}
		it++;
	}
	if (targetFd == -1)
	{
		std::string response = ":server 401 " + client.getNickname() + " " + cmd.getParameter(1) + " :No such nick/channel\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	if (channel.isMember(targetFd) == false)
	{
		std::string response = ":server 441 " + client.getNickname() + " " + it->second.getNickname() + " " + channel.getName() + " :They aren't on that channel\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	std::string message = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " KICK " + channel.getName() + " " + it->second.getNickname() +  " :";
	if (cmd.getParameterCount() == 3)
	{
		message += cmd.getParameter(2);
	}
	message += "\r\n";
	channel.broadcast(message, -1);
	channel.removeMember(targetFd);
}
