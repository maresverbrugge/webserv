/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felicia <felicia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:49:09 by fkoolhov          #+#    #+#             */
/*   Updated: 2024/04/17 12:51:58 by felicia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "ServerPool.hpp"
#include "Server.hpp"
#include "Location.hpp"

static std::vector<std::string> get_words_in_line(std::string line)
{
	std::istringstream stringstream(line);
	std::vector<std::string> words;
	std::string word;

	while (stringstream >> word)
		words.push_back(word);
	return (words);
}

static void handle_location_directive(std::vector<std::string> words)
{
	if (words[0][0] == '#')
	{
		return;
	}
	else if (words[0] == "allow_methods")
	{
	}
	else if (words[0] == "default")
	{
	}
	else if (words[0] == "upload_directory")
	{
	}
	else if (words[0] == "redirect")
	{
	}
	else if (words[0] == "cgi_extension")
	{
	}
	else if (words[0] == "directory_listing")
	{
	}
	else if (words[0] == "path")
	{
	}
	else
	{
		throw std::runtime_error("Unkown directive: " + words[0]);
	}		
}

static std::streampos setup_new_location(std::stack<char> brackets, std::streampos current_position, std::string filepath, std::vector<std::string> current_words)
{
	if (current_words.size() != 2)
	{
		throw std::runtime_error("unkown location directive.");
	}

	std::ifstream infile(filepath);
	std::string line;
	infile.seekg(current_position);
	
	while (std::getline(infile, line))
	{
		std::vector<std::string> words = get_words_in_line(line);
		if (words.size() > 0)
		{
			if (words.size() == 1 && words[0].length() == 1 && words[0][0] == '{')
			{
				brackets.push('{');
				if (brackets.size() != 2)
				{
					infile.close();
					throw std::runtime_error("Too many brackets encountered.");
				}
			}
			else if (words.size() == 1 && words[0].length() == 1 && words[0][0] == '}')
			{
				brackets.pop();
				if (brackets.size() == 1)
				{
					std::streampos updated_position = infile.tellg();
					infile.close();
					return (updated_position);
				}
			}
			else
			{
				handle_location_directive(words);	
			}
		}
	}

	infile.close();
	throw std::runtime_error("No location closing bracket.");
}

static void get_host_from_config(std::unique_ptr<Server> &server, std::vector<std::string> words)
{
	if (words.size() != 2)
	{
		throw std::runtime_error("Invalid host directive.");
	}
	server->setHost(words[1]);
}

static void get_server_names_from_config(std::unique_ptr<Server> &server, std::vector<std::string> words)
{
	if (words.size() < 2)
	{
		throw std::runtime_error("Invalid server_names directive.");
	}
	for (size_t i = 1; i < words.size(); i++)
	{
		if (words[i][0] == '#')
			break;
		server->addServerName(words[i]);
	}
}

static std::streampos handle_server_directive(std::unique_ptr<Server> &server, std::streampos current_position, std::stack<char> brackets, std::string filepath, std::vector<std::string> words)
{
	if (words[0][0] == '#')
	{
		return current_position;
	}
	else if (words[0] == "host")
	{
		get_host_from_config(server, words);
	}
	else if (words[0] == "port")
	{
	}
	else if (words[0] == "server_names")
	{
		get_server_names_from_config(server, words);
	}
	else if (words[0] == "root")
	{
	}
	else if (words[0] == "error_page")
	{
	}
	else if (words[0] == "default_error_page")
	{
	}
	else if (words[0] == "client_max_body_size")
	{
	}
	else if (words[0] == "location")
	{
		std::unique_ptr<Location> location = std::make_unique<Location>();
		std::streampos new_position = setup_new_location(brackets, current_position, filepath, words);
		server->addLocation(std::move(location));
		return new_position;
	}
	else
	{
		throw std::runtime_error("Unkown directive: " + words[0]);
	}

	return (current_position);
}

static std::streampos setup_new_server(std::unique_ptr<Server> &server, std::streampos current_position, std::string filepath, std::vector<std::string> current_words)
{
	if (current_words.size() != 1)
	{
		throw std::runtime_error("Unkown server directive.");
	}

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

static std::string open_infile(char* filepath_arg, std::ifstream& infile)
{
	std::string user_filepath(filepath_arg);
		
	infile.open(user_filepath);
	if (!infile.is_open())
	{
		std::string default_filepath = "./config/default.conf";
		infile.open(default_filepath);
		if (!infile.is_open())
		{
			throw std::runtime_error("Failed to open both the specified file and the default file.");
		}
		return default_filepath;
	}
	else
	{
		return user_filepath;
	}
}

void configure_servers(char* filepath_arg)
{
	std::ifstream infile;
	
	try
	{
		std::string filepath = open_infile(filepath_arg, infile);
		std::unique_ptr<ServerPool> server_pool = std::make_unique<ServerPool>();
		std::string line;
		
		while (std::getline(infile, line))
		{
			std::vector<std::string> words = get_words_in_line(line);
	
			if (words.size() > 0)
			{
				if (words[0][0] == '#')
				{
					continue;
				}
				else if (words[0] == "server")
				{
					std::unique_ptr<Server> server = std::make_unique<Server>();
					std::streampos current_position = infile.tellg();
					std::streampos new_position = setup_new_server(server, current_position, filepath, words);
					server_pool->addServer(std::move(server));
					std::cout << *server_pool->getServers().back() << std::endl;
					infile.seekg(new_position);
				}
			}
		}
		
		infile.close();
	}
	catch (const std::exception& exception)
	{
		infile.close();
		std::cerr << exception.what() << std::endl;
		// free mem?
		exit(EXIT_FAILURE);
	}
}