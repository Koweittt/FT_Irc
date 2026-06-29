/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abignals <abignals@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 16:52:30 by abignals          #+#    #+#             */
/*   Updated: 2026/06/29 16:52:31 by abignals         ###   ########.fr       */
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
		std::string response = ":server 461 * USER :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	std::string username = cmd.getParameter(0);

	if (client.hasPassword() == false)
	{
		std::string response = ":server 464 * :Password required\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	client.setUsername(username);

	if (client.hasPassword() && client.hasNickname() && client.hasUsername() && !client.isRegistered())
	{
		client.setRegistered(true);
		std::string response = ":server 001 " + client.getNickname() + " :Welcome to IRC\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
}
