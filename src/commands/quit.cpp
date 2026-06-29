/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/26 02:01:15 by koweit           ###   ########.fr       */
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

