/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/26 04:16:31 by koweit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>
#include <cstdlib>

void handleMode(const command &cmd, Client &client, Server &server)
{
	if (cmd.getParameterCount() < 2)
	{
		std::string response = "461 " + client.getNickname() + " MODE :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	if (server.getChannels().find(cmd.getParameter(0)) == server.getChannels().end())
	{
		std::string response = "403 :Channel doesn't exist\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	Channel& channel = server.getChannels()[cmd.getParameter(0)];
	if (channel.isOperator(client.getFd()) == false)
	{
		std::string response = "482 :You are not operator\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	std::string modes = cmd.getParameter(1);
	bool adding = false;
	int argIndex = 2;
	for (int i = 0; modes[i]; i++)
	{
		char c = modes[i];
		if (c == '+')
			adding = true;
		else if (c == '-')
			adding = false;
		else if (c == 'i')
			channel.setInviteOnly(adding);
		else if (c == 't')
			channel.setTopicRestricted(adding);
		else if (c == 'k')
		{
			if (adding)
			{
				if (argIndex < cmd.getParameterCount())
				{
					channel.setKey(cmd.getParameter(argIndex++));
				}
				else
					break;
			}
			else
				channel.setKey("");
		}
		else if (c == 'l')
		{
			if (adding)
			{
				if (argIndex < cmd.getParameterCount())
					channel.setUserLimit(atoi(cmd.getParameter(argIndex++).c_str()));
				else
					break;
			}
			else
				channel.setUserLimit(0);
		}
		else if (c == 'o')
		{
			if (argIndex < cmd.getParameterCount())
			{
				int targetFd = -1;
				std::map<int, Client>::iterator it = server.getClients().begin();
				while (it != server.getClients().end())
				{
					if (it->second.getNickname() == cmd.getParameter(argIndex))
					{
						targetFd = it->first;
						argIndex++;
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
				if (adding)
					channel.addOperator(targetFd);
				else
					channel.removeOperator(targetFd);
			}
			else
				break;
		}
	}

	std::string response = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " MODE " + channel.getName() + modes + "\r\n";
	channel.broadcast(response, -1);
}
