/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felicia <felicia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:50:59 by fkoolhov          #+#    #+#             */
/*   Updated: 2024/04/18 19:05:30 by felicia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include "ServerPool.hpp"
# include "Server.hpp"
# include "Location.hpp"

// configServerPool.cpp
void configure_serverpool(char* filepath);

// configServer.cpp
std::streampos configure_server(std::unique_ptr<Server>& server, std::streampos current_position, std::string filepath, std::vector<std::string> current_words);

// configLocation.cpp
std::streampos configure_location(std::unique_ptr<Location>& location, std::stack<char> brackets, std::streampos current_position, std::string filepath, std::vector<std::string> current_words);

// configUtils.cpp
std::vector<std::string> get_words_in_line(std::string line);

#endif