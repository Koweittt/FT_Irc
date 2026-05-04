/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trambure <trambure@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 12:11:00 by trambure          #+#    #+#             */
/*   Updated: 2026/05/04 15:17:46 by trambure         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

command::command() {}

command::~command() {}

std::vector<std::string> command::getParams() const
{
	return (_params);
}

std::string command::getCmd() const
{
	return (_cmd);
}

void command::parseMessage(std::string msg)
{
    std::size_t pos = msg.find_first_not_of(" \t\n\v\f\r");
    msg = msg.substr(pos);
	pos = msg.find_last_not_of(" \t\n\v\f\r");
	msg = msg.substr(0, pos + 1);
	pos = msg.find(' ');
	_cmd = msg.substr(0, pos);
	msg = msg.substr(pos + 1);
	pos = 0;
	while(msg[pos] != '\0')
	{
		if (msg[0] == ':')
		{
			_params.push_back(msg.substr(1));
			break;
		}
		else
		{
			pos = msg.find(' ');
			if (pos == std::string::npos)
			{
				_params.push_back(msg);
				break;
			}
			_params.push_back(msg.substr(0, pos));
			msg = msg.substr(pos + 1);
		}
	}
}