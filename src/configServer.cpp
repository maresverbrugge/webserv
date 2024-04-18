/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felicia <felicia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:27:26 by felicia           #+#    #+#             */
/*   Updated: 2024/04/18 19:29:59 by felicia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configuration.hpp"

static void get_client_max_body_size_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid client_max_body_size directive.");

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

	unsigned long long max_body_size = std::stoull(max_body_size_str) * multiplier;
	server->setClientMaxBodySize(max_body_size);
}

static void get_custom_error_page_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 3)
		throw std::runtime_error("Invalid custom_error_page directive.");
	server->addCustomErrorPage(std::stoi(words[1]), words[2]);
} // should be able to handle multiple custom error pages in one line

static void get_default_error_page_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid default_error_page directive.");
	server->setDefaultErrorPage(words[1]);
}

static void get_root_folder_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid root directive.");
	server->setRootFolder(words[1]);
}

static void get_server_names_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid server_names directive.");
	for (size_t i = 1; i < words.size(); i++)
	{
		if (words[i][0] == '#')
			break;
		server->addServerName(words[i]);
	}
}

static void get_host_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid host directive.");
	server->setHost(words[1]);
}

static void get_port_from_config(std::unique_ptr<Server>& server, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid port directive.");
	server->setPort(std::stoi(words[1]));
}

static std::streampos handle_server_directive(std::unique_ptr<Server>& server, std::streampos current_position, std::stack<char> brackets, std::string filepath, std::vector<std::string> words)
{
	if (words[0][0] == '#')
		return current_position;
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
	{
		std::unique_ptr<Location> location = std::make_unique<Location>();
		std::streampos new_position = configure_location(location, brackets, current_position, filepath, words);
		server->addLocation(std::move(location));
		return new_position;
	}
	else
	{
		throw std::runtime_error("Unknown directive: " + words[0]);
	}

	return current_position;
}

std::streampos configure_server(std::unique_ptr<Server>& server, std::streampos current_position, std::string filepath, std::vector<std::string> current_words)
{
	if (current_words.size() != 1)
		throw std::runtime_error("Unkown server directive.");

	std::ifstream infile(filepath);
	std::string line;
	std::stack<char> brackets;
	infile.seekg(current_position);
	
	while (std::getline(infile, line))
	{
		std::vector<std::string> words = get_words_in_line(line);
		if (words.size() > 0)
		{
			std::streampos current_position = infile.tellg();
			if (words.size() == 1 && words[0] == "{")
			{
				brackets.push('{');
				if (brackets.size() != 1)
				{
					infile.close();
					throw std::runtime_error("Too many brackets encountered.");
				}
			}
			else if (words.size() == 1 && words[0] == "}")
			{
				brackets.pop();
				if (brackets.size() == 0)
				{
					infile.close();
					return (current_position);
				}
			}
			else
			{
				std::streampos new_position = handle_server_directive(server, current_position, brackets, filepath, words);
				infile.seekg(new_position);
			}
		}
	}
	
	infile.close();
	return std::string::npos;
}