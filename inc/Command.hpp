/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trambure <trambure@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 11:58:16 by trambure          #+#    #+#             */
/*   Updated: 2026/06/23 15:52:00 by trambure         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstddef>

class Client;
class Server;

class command
{
	private:
		std::string					_cmd;
		std::vector<std::string>	_params;
	public:
		command(const std::string &msg);
		~command();

		void						parseMessage(const std::string &msg);
		std::string					getCmd() const;
		std::vector<std::string>	getParams() const;
		int							getParameterCount() const;
		const std::string			&getParameter(size_t index) const;
};

// Authentication commands
void handlePass(const command &cmd, Client &client, Server &server);
void handleNick(const command &cmd, Client &client, Server &server);
void handleUser(const command &cmd, Client &client, Server &server);

// User commands
void handleJoin(const command &cmd, Client &client, Server &server);
void handlePrivmsg(const command &cmd, Client &client, Server &server);
void handleNotice(const command &cmd, Client &client, Server &server);
void handlePart(const command &cmd, Client &client, Server &server);
void handleQuit(const command &cmd, Client &client, Server &server);

// Operator commands
void handleKick(const command &cmd, Client &client, Server &server);
void handleInvite(const command &cmd, Client &client, Server &server);
void handleTopic(const command &cmd, Client &client, Server &server);
void handleMode(const command &cmd, Client &client, Server &server);

// Dispatcher
void executeCommand(const command &cmd, Client &client, Server &server);

#endif