/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:50:59 by fkoolhov          #+#    #+#             */
/*   Updated: 2024/04/20 16:23:11 by fkoolhov         ###   ########.fr       */
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

// configErrors.cpp
void check_location_config_errors(std::unique_ptr<Location>& location);
void check_server_config_errors(std::unique_ptr<Server>& server);
void check_serverpool_config_errors(std::unique_ptr<ServerPool>& serverpool);

#endif