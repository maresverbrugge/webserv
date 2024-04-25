/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:27:26 by felicia           #+#    #+#             */
/*   Updated: 2024/04/25 14:39:05 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configuration.hpp"

static void get_client_max_body_size_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for client_max_body_size directive.");
	else
	{
		int multiplier = 1;
		std::string max_body_size_str = words[1];
		if (max_body_size_str.back() == 'k' || max_body_size_str.back() == 'K') 
		{
			multiplier = 1024;
			max_body_size_str.pop_back();
		} 
		else if (max_body_size_str.back() == 'm' || max_body_size_str.back() == 'M') 
		{
			multiplier = 1024 * 1024;
			max_body_size_str.pop_back();
		}

		try
		{
			unsigned long long max_body_size = std::stoull(max_body_size_str) * multiplier;
			server->setClientMaxBodySize(max_body_size);
		}
		catch (const std::exception& exception)
		{
			config_error_message("Invalid client_max_body_size: " + words[1] + ": " + exception.what());
		}
	}
}

static void get_custom_error_page_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 3)
		config_error_message("Invalid number of arguments for error_page directive.");
	else
	{
		unsigned long i = 1;
		for (; i < words.size() && words[i][0] != '#'; i++)
			;
		i--;
		std::string error_page = words[i];
		for (unsigned long j = 1; j < i; j++)
			server->addCustomErrorPage(static_cast<short>(std::stoi(words[j])), error_page);
	}
}

static void get_default_error_page_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for default_error_page directive.");
	else
		server->setDefaultErrorPage(words[1]);
}

static void get_root_folder_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for root directive.");
	else
		server->setRootFolder(words[1]);
}

static void get_server_names_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for server_names directive.");
	else
	{
		for (size_t i = 1; i < words.size(); i++)
		{
			if (words[i][0] == '#')
				break;
			server->addServerName(words[i]);
		}
	}
}

static void get_host_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for host directive.");
	else
		server->setHost(words[1]);
}

static void get_port_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for port directive.");
	else
	{
		try
		{
			server->setPort(std::stoull(words[1]));
		}
		catch (const std::exception& exception)
		{
			config_error_message("Invalid port number: " + words[1] + ": " + exception.what());
		}
	}
}

static bool get_location_name_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for location directive.");
	else if (words[1] == "/")
		return true;
	else
		location->setLocationName(words[1]);
	return false;
}

static void create_new_location_object(std::unique_ptr<Server>& server, std::ifstream& infile, std::vector<std::string> words)
{
	std::unique_ptr<Location> location = std::make_unique<Location>();

	bool is_default_location = get_location_name_from_config(location, words);
	if (is_default_location && server->getDefaultLocation())
		throw std::runtime_error("Server can have only one default location.");
	int config_error = configure_location(location, infile, words, server->getRootFolder(), is_default_location);
	
	if (config_error == EXIT_SUCCESS && is_default_location)
		server->setDefaultLocation(std::move(location));
	else if (config_error == EXIT_SUCCESS)
		server->addLocation(std::move(location));
}

// Checks the current server directive (or comment or invalid directive)
static void handle_server_directive(std::unique_ptr<Server>& server, std::ifstream& infile, std::vector<std::string> words)
{
	if (words[0][0] == '#')
		return;
	else if (words[0] == "port")
		get_port_from_config(server, words);
	else if (words[0] == "host")
		get_host_from_config(server, words);
	else if (words[0] == "server_names")
		get_server_names_from_config(server, words);
	else if (words[0] == "root")
		get_root_folder_from_config(server, words);
	else if (words[0] == "default_error_page")
		get_default_error_page_from_config(server, words);
	else if (words[0] == "error_page")
		get_custom_error_page_from_config(server, words);
	else if (words[0] == "client_max_body_size")
		get_client_max_body_size_from_config(server, words);
	else if (words[0] == "location")
		create_new_location_object(server, infile, words);
	else
		config_error_message("Unknown server directive: " + words[0]);
}

// Adds the root folder to server filepaths
static void create_full_server_paths(std::unique_ptr<Server>& server)
{
	server->setRootFolder("./" + server->getRootFolder());
	server->setDefaultErrorPage(server->getRootFolder() + server->getDefaultErrorPage());
	std::map<short, std::string> custom_error_pages = server->getCustomErrorPages();
	for (auto it = custom_error_pages.begin(); it != custom_error_pages.end(); it++)
		server->addCustomErrorPage(it->first, server->getRootFolder() + it->second);
}

// Reads the server section of the config file and configures a server object
int configure_server(std::unique_ptr<Server>& server, std::ifstream& infile, std::vector<std::string> words)
{
	std::string line;
	std::stack<char> brackets;
	
	while (std::getline(infile, line))
	{
		words = get_words_in_line(line);
		if (words.size() > 0)
		{
			bool found_bracket = check_for_brackets(words, brackets);
			if (!found_bracket)
				handle_server_directive(server, infile, words);
			else if (found_bracket && brackets.size() == 0)
			{
				int config_error = check_server_config_errors(server);
				create_full_server_paths(server);
				return config_error;
			}
		}
	}
	throw std::runtime_error("Server should have closing bracket.");
}