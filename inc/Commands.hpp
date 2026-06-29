/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 00:00:00 by koweit            #+#    #+#             */
/*   Updated: 2026/06/04 00:00:00 by koweit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"

void cmdJoin   (Server& server, Client& client, Command& cmd);
void cmdPrivmsg(Server& server, Client& client, Command& cmd);
void cmdKick   (Server& server, Client& client, Command& cmd);
void cmdInvite (Server& server, Client& client, Command& cmd);
void cmdTopic  (Server& server, Client& client, Command& cmd);
void cmdMode   (Server& server, Client& client, Command& cmd);

#endif
