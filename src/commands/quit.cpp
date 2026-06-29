/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abignals <abignals@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 16:52:22 by abignals          #+#    #+#             */
/*   Updated: 2026/06/29 16:52:23 by abignals         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void handleQuit(const command &cmd, Client &client, Server &server)
{
	std::string message = "";
	if (cmd.getParameterCount() < 1)
	{
	 	message = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " QUIT \r\n";
	}
	else
	{
		message = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " QUIT :" + cmd.getParameter(0) + "\r\n";
	}

	std::map<std::string, Channel>::iterator it = server.getChannels().begin();
	while (it != server.getChannels().end())
	{
		if (it->second.isMember(client.getFd()))
		{
			it->second.broadcast(message, client.getFd());
			it->second.removeMember(client.getFd());
			if (it->second.getMemberCount() == 0)
			{
				server.getChannels().erase(it++);
				continue;
			}
		}
		++it;
	}
	server.disconnectClient(client.getFd());
}

