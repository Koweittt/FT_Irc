/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abignals <abignals@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 16:52:43 by abignals          #+#    #+#             */
/*   Updated: 2026/06/29 16:52:44 by abignals         ###   ########.fr       */
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
	else if (cmdName == "PING")
	{
		std::string token = cmd.getParameterCount() > 0 ? cmd.getParameter(0) : "server";
		std::string response = "PONG :" + token + "\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	else if (cmdName == "CAP")
	{
		if (cmd.getParameterCount() >= 1 && cmd.getParameter(0) == "LS")
		{
			std::string response = "CAP * LS :\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);
		}
		return;
	}

	if (!client.isRegistered())
	{
		std::string response = ":server 451 * :You have not registered\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	if (cmdName == "JOIN")
		handleJoin(cmd, client, server);
	else if (cmdName == "PRIVMSG")
		handlePrivmsg(cmd, client, server);
	else if (cmdName == "NOTICE")
		handleNotice(cmd, client, server);
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
	else if (cmdName == "WHO")
	{
		std::string target = cmd.getParameterCount() > 0 ? cmd.getParameter(0) : "*";
		std::string response = ":server 315 " + client.getNickname() + " " + target + " :End of WHO list\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
	else
	{
		std::string response = ":server 421 " + client.getNickname() + " " + cmdName + " :Unknown command\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
	}
}
