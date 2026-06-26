/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/25 23:05:43 by koweit           ###   ########.fr       */
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
	
	if (client.hasPassword() == false)
	{
		std::string response = "464 " + username + " have no password\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	client.setUsername(username);

	if (client.hasPassword() && client.hasNickname() && client.hasUsername())
	{
		client.setRegistered(true);
		std::string response = "001 " + client.getNickname() + " :Welcome to IRC\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
}
