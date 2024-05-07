/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   configuration.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: fkoolhov <fkoolhov@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/16 14:50:59 by fkoolhov      #+#    #+#                 */
/*   Updated: 2024/05/07 11:27:22 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include "ServerPool.hpp"
# include "Server.hpp"
# include "Location.hpp"

// configServerPool.cpp
std::unique_ptr<ServerPool> configure_serverpool(char* filepath);

// configServer.cpp
int configure_server(std::shared_ptr<Server>& server, std::ifstream& infile, std::vector<std::string> words);

// configLocation.cpp
int configure_location(std::shared_ptr<Location>& location, std::ifstream& infile, std::vector<std::string> words, std::string root_folder, bool is_default_location);

// configUtils.cpp
std::vector<std::string> get_words_in_line(std::string line);
bool check_for_brackets(std::vector<std::string> words, std::stack<char>& brackets);
void config_error_message(std::string message);

// configErrors.cpp
int check_location_config_errors(std::shared_ptr<Location>& location, bool is_default_location);
int check_server_config_errors(std::shared_ptr<Server>& server);
void check_serverpool_config_errors(std::unique_ptr<ServerPool>& serverpool);

#endif