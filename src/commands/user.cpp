/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trambure <trambure@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/23 15:57:18 by trambure         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void handleUser(const command &cmd, Client &client, Server &server)
{
	(void)server;

	if (cmd.getParameterCount() < 4)
	{
		std::string response = "461 USER :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	std::string username = cmd.getParameter(0);
	client.setUsername(username);

	if (client.hasPassword() && client.hasNickname() && client.hasUsername())
	{
		client.setRegistered(true);
		std::string response = "001 " + client.getNickname() + " :Welcome to IRC\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
}
