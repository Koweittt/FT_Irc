/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
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

void handlePart(const command &cmd, Client &client, Server &server)
{
	(void)server;

	if (cmd.getParameterCount() < 1)
	{
		std::string response = "461 " + client.getNickname() + " PART :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	std::string channel = cmd.getParameter(0);
	std::string response = ":" + client.getNickname() + " PART " + channel + "\r\n";
	send(client.getFd(), response.c_str(), response.length(), 0);
}
