/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abignals <abignals@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 16:52:14 by abignals          #+#    #+#             */
/*   Updated: 2026/06/29 16:52:15 by abignals         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void handlePass(const command &cmd, Client &client, Server &server)
{
	if (client.isRegistered())
	{
		std::string response = ":server 462 " + client.getNickname() + " :You may not reregister\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	if (cmd.getParameterCount() < 1)
	{
		std::string response = ":server 461 * PASS :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	if (cmd.getParameter(0) != server.getPass())
	{
		std::string response = ":server 464 * :Password incorrect\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		server.disconnectClient(client.getFd());
		return;
	}

	client.setPassword(cmd.getParameter(0));
}
