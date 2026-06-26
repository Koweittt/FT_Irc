/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/25 23:00:08 by koweit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void handleNick(const command &cmd, Client &client, Server &server)
{
	if (cmd.getParameterCount() < 1)
	{
		std::string response = "461 NICK :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	std::string nick = cmd.getParameter(0);
	if (nick.empty() || nick.length() > 9)
	{
		std::string response = "432 " + nick + " :Erroneous nickname\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	std::map<int, Client>::iterator it = server.getClients().begin();
	while (it != server.getClients().end())
	{
		if (it->second.getNickname() == nick && it->first != client.getFd())
		{
			std::string response = "433 " + nick + " :is already in use\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);
			return;
		}
		++it;
	}

	client.setNickname(nick);
}
