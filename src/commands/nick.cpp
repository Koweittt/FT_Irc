/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abignals <abignals@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/29 00:07:00 by abignals         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void handleNick(const command &cmd, Client &client, Server &server)
{
	std::string self = client.getNickname().empty() ? "*" : client.getNickname();

	if (cmd.getParameterCount() < 1)
	{
		std::string response = ":server 431 " + self + " :No nickname given\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	std::string nick = cmd.getParameter(0);
	if (nick.empty() || nick.length() > 9)
	{
		std::string response = ":server 432 " + self + " " + nick + " :Erroneous nickname\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	std::map<int, Client>::iterator it = server.getClients().begin();
	while (it != server.getClients().end())
	{
		if (it->second.getNickname() == nick && it->first != client.getFd())
		{
			std::string response = ":server 433 " + self + " " + nick + " :Nickname is already in use\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);
			return;
		}
		++it;
	}

	std::string oldNick = client.getNickname();
	client.setNickname(nick);

	if (client.hasPassword() && client.hasNickname() && client.hasUsername() && !client.isRegistered())
	{
		client.setRegistered(true);
		std::string response = ":server 001 " + client.getNickname() + " :Welcome to IRC\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
	else if (client.isRegistered())
	{
		std::string nickMsg = ":" + oldNick + "!" + client.getUsername() + "@" + client.getIpAddr() + " NICK :" + nick + "\r\n";
		send(client.getFd(), nickMsg.c_str(), nickMsg.length(), 0);
		std::map<std::string, Channel>::iterator it = server.getChannels().begin();
		while (it != server.getChannels().end())
		{
			if (it->second.isMember(client.getFd()))
				it->second.broadcast(nickMsg, client.getFd());
			++it;
		}
	}
}
