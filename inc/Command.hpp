/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trambure <trambure@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 11:58:16 by trambure          #+#    #+#             */
/*   Updated: 2026/05/04 12:26:42 by trambure         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define COMMAND_HPP
#ifndef COMMAND_HPP

#include <iostream>
#include <string>
#include <cctype>
#include <cstddef> 

class command
{
	private:
		std::string					_cmd;
		std::vector<std::string>	_params;
	public:
		command();
		~command();

		void						parseMessage(std::string msg);
		std::string					getCmd() const;
		std::vector<std::string>	getParams() const;
};

#endif