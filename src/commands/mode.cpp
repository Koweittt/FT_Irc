/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abignals <abignals@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 16:51:58 by abignals          #+#    #+#             */
/*   Updated: 2026/06/29 16:51:59 by abignals         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>
#include <cstdlib>

void handleMode(const command &cmd, Client &client, Server &server)
{
	if (cmd.getParameterCount() < 1)
	{
		std::string response = ":server 461 " + client.getNickname() + " MODE :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	if (cmd.getParameterCount() == 1)
	{
		if (cmd.getParameter(0)[0] != '#')
		{
			std::string response = ":server 221 " + client.getNickname() + " :+\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);
			return;
		}
		if (server.getChannels().find(cmd.getParameter(0)) == server.getChannels().end())
		{
			std::string response = ":server 403 " + client.getNickname() + " " + cmd.getParameter(0) + " :No such channel\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);
			return;
		}
		Channel& ch = server.getChannels()[cmd.getParameter(0)];
		std::string modestr = "+";
		if (ch.isInviteOnly())     modestr += "i";
		if (ch.isTopicRestricted()) modestr += "t";
		if (ch.getKey() != "")     modestr += "k";
		if (ch.getUserLimit() != 0) modestr += "l";
		std::string response = ":server 324 " + client.getNickname() + " " + cmd.getParameter(0) + " " + modestr + "\r\n";
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
	std::string modes = cmd.getParameter(1);
	if (modes[0] != '+' && modes[0] != '-')
	{
		if (modes.find('b') != std::string::npos)
		{
			std::string response = ":server 368 " + client.getNickname() + " " + channel.getName() + " :End of channel ban list\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);
		}
		return;
	}
	if (channel.isOperator(client.getFd()) == false)
	{
		std::string response = ":server 482 " + client.getNickname() + " " + channel.getName() + " :You're not channel operator\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	bool adding = false;
	bool modeApplied = false;
	std::string appliedArgs = "";
	int argIndex = 2;
	for (int i = 0; modes[i]; i++)
	{
		char c = modes[i];
		if (c == '+')
			adding = true;
		else if (c == '-')
			adding = false;
		else if (c == 'i')
		{ channel.setInviteOnly(adding); modeApplied = true; }
		else if (c == 't')
		{ channel.setTopicRestricted(adding); modeApplied = true; }
		else if (c == 'k')
		{
			if (adding)
			{
				if (argIndex < cmd.getParameterCount())
				{
					appliedArgs += " " + cmd.getParameter(argIndex);
					channel.setKey(cmd.getParameter(argIndex++));
					modeApplied = true;
				}
				else
					break;
			}
			else
			{ channel.setKey(""); modeApplied = true; }
		}
		else if (c == 'l')
		{
			if (adding)
			{
				if (argIndex < cmd.getParameterCount())
				{
					appliedArgs += " " + cmd.getParameter(argIndex);
					channel.setUserLimit(atoi(cmd.getParameter(argIndex++).c_str()));
					modeApplied = true;
				}
				else
					break;
			}
			else
			{ channel.setUserLimit(0); modeApplied = true; }
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
						break;
					}
					++it;
				}
				if (targetFd == -1)
				{
					std::string response = ":server 401 " + client.getNickname() + " " + cmd.getParameter(argIndex) + " :No such nick/channel\r\n";
					send(client.getFd(), response.c_str(), response.length(), 0);
					return;
				}
				appliedArgs += " " + cmd.getParameter(argIndex++);
				if (adding)
					channel.addOperator(targetFd);
				else
					channel.removeOperator(targetFd);
				modeApplied = true;
			}
			else
				break;
		}
	}

	if (modeApplied)
	{
		std::string response = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " MODE " + channel.getName() + " " + modes + appliedArgs + "\r\n";
		channel.broadcast(response, -1);
	}
}
