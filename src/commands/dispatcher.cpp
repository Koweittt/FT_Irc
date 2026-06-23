/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trambure <trambure@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/23 15:57:01 by trambure         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void executeCommand(const command &cmd, Client &client, Server &server)
{
	std::string cmdName = cmd.getCmd();

	if (cmdName == "PASS")
	{
		handlePass(cmd, client, server);
		return;
	}
	else if (cmdName == "NICK")
	{
		handleNick(cmd, client, server);
		return;
	}
	else if (cmdName == "USER")
	{
		handleUser(cmd, client, server);
		return;
	}

	if (!client.isRegistered())
	{
		std::string response = "ERROR :You must be registered first\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	if (cmdName == "JOIN")
		handleJoin(cmd, client, server);
	else if (cmdName == "PRIVMSG")
		handlePrivmsg(cmd, client, server);
	else if (cmdName == "PART")
		handlePart(cmd, client, server);
	else if (cmdName == "QUIT")
		handleQuit(cmd, client, server);

	else if (cmdName == "KICK")
		handleKick(cmd, client, server);
	else if (cmdName == "INVITE")
		handleInvite(cmd, client, server);
	else if (cmdName == "TOPIC")
		handleTopic(cmd, client, server);
	else if (cmdName == "MODE")
		handleMode(cmd, client, server);

	else
	{
		std::string response = "421 " + client.getNickname() + " " + cmdName + " :Unknown command\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
}
