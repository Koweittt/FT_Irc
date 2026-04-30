/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koweit <koweit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 11:49:45 by koweit            #+#    #+#             */
/*   Updated: 2026/04/26 11:50:32 by koweit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client
{
    public:
        Client();
        Client(const Client& src);
        Client& operator=(const Client& src);
        ~Client();
        
    private:
};

#endif //CLIENT_HPP