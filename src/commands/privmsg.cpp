/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trambure <trambure@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/23 15:44:36 by trambure         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void handlePrivmsg(const command &cmd, Client &client, Server &server)
{
	(void)server;

	if (cmd.getParameterCount() < 2)
	{
		std::string response = "461 " + client.getNickname() + " PRIVMSG :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	std::string target = cmd.getParameter(0);
	std::string message = cmd.getParameter(1);
	std::string response = ":" + client.getNickname() + " PRIVMSG " + target + " :" + message + "\r\n";
	send(client.getFd(), response.c_str(), response.length(), 0);
}
