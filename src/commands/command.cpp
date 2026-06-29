/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abignals <abignals@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 16:52:48 by abignals          #+#    #+#             */
/*   Updated: 2026/06/29 16:52:49 by abignals         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../inc/Command.hpp"

command::command(const std::string &msg) : _cmd(""), _params()
{
	parseMessage(msg);
}

command::~command() {}

std::vector<std::string> command::getParams() const
{
	return (_params);
}

std::string command::getCmd() const
{
	return (_cmd);
}

void command::parseMessage(const std::string &rawMsg)
{
	std::string msg = rawMsg;
	std::size_t pos = msg.find_first_not_of(" \t\n\v\f\r");
	if (pos == std::string::npos)
	{
		_cmd = "";
		_params.clear();
		return;
	}
	msg = msg.substr(pos);
	pos = msg.find_last_not_of(" \t\n\v\f\r");
	msg = msg.substr(0, pos + 1);
	
	if (msg.empty())
	{
		_cmd = "";
		_params.clear();
		return;
	}
	
	pos = msg.find(' ');
	_cmd = msg.substr(0, pos);
	
	// Convert command to uppercase
	for (size_t i = 0; i < _cmd.length(); i++)
		_cmd[i] = std::toupper(_cmd[i]);
	
	if (pos == std::string::npos)
	{
		_params.clear();
		return;
	}
	
	msg = msg.substr(pos + 1);
	_params.clear();
	

	while (!msg.empty())
	{
		pos = msg.find_first_not_of(" \t");
		if (pos == std::string::npos)
			break;
		msg = msg.substr(pos);
		
		if (msg[0] == ':')
		{
			_params.push_back(msg.substr(1));
			break;
		}
		
		pos = msg.find(' ');
		if (pos == std::string::npos)
		{
			_params.push_back(msg);
			break;
		}
		
		_params.push_back(msg.substr(0, pos));
		msg = msg.substr(pos);
	}
}

int command::getParameterCount() const
{
	return (_params.size());
}

const std::string &command::getParameter(size_t index) const
{
	static std::string empty = "";
	if (index >= _params.size())
		return (empty);
	return (_params[index]);
}