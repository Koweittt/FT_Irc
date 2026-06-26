/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/26 03:48:20 by koweit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include <sys/socket.h>

void handleTopic(const command &cmd, Client &client, Server &server)
{
	

	if (cmd.getParameterCount() < 1)
	{
		std::string response = "461 " + client.getNickname() + " TOPIC :Not enough parameters\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}

	if (server.getChannels().find(cmd.getParameter(0)) == server.getChannels().end())
	{
		std::string response = "403 :Channel doesn't exist\r\n";
		send(client.getFd(), response.c_str(), response.length(), 0);
		return;
	}
	
	Channel& channel = server.getChannels()[cmd.getParameter(0)];
	if (cmd.getParameterCount() == 1)
	{
		std::string message = "332 " + client.getNickname() + " " + cmd.getParameter(0) + " :";
		if (channel.getTopic() != "")
		{
			message += channel.getTopic() + "\r\n";
		}
		else
		{
			message = "331 " + client.getNickname() + " " + cmd.getParameter(0) + " :No topic is set\r\n";
		}
		send(client.getFd(), message.c_str(), message.length(), 0);
	}
	if (cmd.getParameterCount() == 2)
	{
		if (channel.isMember(client.getFd()) == false)
		{
			std::string response = "442 :You are not member\r\n";
			send(client.getFd(), response.c_str(), response.length(), 0);
			return;
		}
		if (channel.isTopicRestricted() == true)
		{
			if (channel.isOperator(client.getFd()) == false)
			{
				std::string response = "482 :You are not operator\r\n";
				send(client.getFd(), response.c_str(), response.length(), 0);
				return;				
			}
		}
		channel.setTopic(cmd.getParameter(1));
		std::string response = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddr() + " TOPIC " + channel.getName() + " :" + cmd.getParameter(1) +  "\r\n";
		channel.broadcast(response, -1);
	}
}
