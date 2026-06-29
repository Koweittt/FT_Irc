/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/26 01:26:32 by koweit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void handlePrivmsg(const command &cmd, Client &client, Server &server)
{
	if (cmd.getParameterCount() < 2)
	{
		std::string response = "461 " + client.getNickname() + " PRIVMSG :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	if (cmd.getParameter(0)[0] == '#')
	{
		if (server.getChannels().find(cmd.getParameter(0)) == server.getChannels().end())
		{
			std::string message = ":server 403 " + client.getNickname() + " " + cmd.getParameter(0) + " :No such channel\r\n";
			send(client.getFd(), message.c_str(), message.length(), 0);
			return;
		}
		Channel &channel = server.getChannels()[cmd.getParameter(0)];
		if (!channel.isMember(client.getFd()))
		{
			std::string message = ":server 404 " + client.getNickname() + " " + cmd.getParameter(0) + " :Cannot send to channel\r\n";
			send(client.getFd(), message.c_str(), message.length(), 0);
			return;
		}
		std::string message = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " PRIVMSG " + cmd.getParameter(0) + " :" + cmd.getParameter(1) + "\r\n";
		channel.broadcast(message, client.getFd());
	}
	else
	{
		std::map<int, Client>::iterator it = server.getClients().begin();
		while (it != server.getClients().end())
		{
			if (it->second.getNickname() == cmd.getParameter(0))
			{
				std::string message = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " PRIVMSG " + cmd.getParameter(0) + " :" + cmd.getParameter(1) + "\r\n";
				send(it->second.getFd(), message.c_str(), message.length(), 0);
				return;
			}
			++it;
		}
		std::string message = ":server 401 " + client.getNickname() + " " + cmd.getParameter(0) + " :No such nick/channel\r\n";
		send(client.getFd(), message.c_str(), message.length(), 0);
		return;
	}
}

void handleNotice(const command &cmd, Client &client, Server &server)
{
	if (cmd.getParameterCount() < 2)
		return;

	if (cmd.getParameter(0)[0] == '#')
	{
		if (server.getChannels().find(cmd.getParameter(0)) == server.getChannels().end())
			return;
		Channel &channel = server.getChannels()[cmd.getParameter(0)];
		if (!channel.isMember(client.getFd()))
			return;
		std::string message = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " NOTICE " + cmd.getParameter(0) + " :" + cmd.getParameter(1) + "\r\n";
		channel.broadcast(message, client.getFd());
	}
	else
	{
		std::map<int, Client>::iterator it = server.getClients().begin();
		while (it != server.getClients().end())
		{
			if (it->second.getNickname() == cmd.getParameter(0))
			{
				std::string message = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " NOTICE " + cmd.getParameter(0) + " :" + cmd.getParameter(1) + "\r\n";
				send(it->second.getFd(), message.c_str(), message.length(), 0);
				return;
			}
			++it;
		}
	}
}