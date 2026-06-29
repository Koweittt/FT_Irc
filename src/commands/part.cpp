/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abignals <abignals@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 16:52:10 by abignals          #+#    #+#             */
/*   Updated: 2026/06/29 16:52:11 by abignals         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void handlePart(const command &cmd, Client &client, Server &server)
{
	if (cmd.getParameterCount() < 1)
	{
		std::string response = "461 " + client.getNickname() + " PART :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	if (server.getChannels().find(cmd.getParameter(0)) == server.getChannels().end())
	{
		std::string response = ":server 403 " + client.getNickname() + " " + cmd.getParameter(0) + " :No such channel\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	Channel& channel = server.getChannels()[cmd.getParameter(0)];
	if (channel.isMember(client.getFd()) == false)
	{
		std::string response = ":server 442 " + client.getNickname() + " " + cmd.getParameter(0) + " :You're not on that channel\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	
	std::string message = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " PART " + channel.getName() + "\r\n";
	channel.broadcast(message, -1);
	channel.removeMember(client.getFd());
	if (channel.getMemberCount() == 0)
		server.getChannels().erase(channel.getName());

}
