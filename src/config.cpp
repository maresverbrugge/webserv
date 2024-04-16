/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:49:09 by fkoolhov          #+#    #+#             */
/*   Updated: 2024/04/16 18:03:50 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

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
		std::cout << "directive: allow_methods\n";
	}
	else if (words[0] == "default")
	{
		std::cout << "directive: default\n";
	}
	else if (words[0] == "upload_directory")
	{
		std::cout << "directive: upload_directory\n";
	}
	else if (words[0] == "redirect")
	{
		std::cout << "directive: redirect\n";
	}
	else if (words[0] == "cgi_extension")
	{
		std::cout << "directive: cgi_extension\n";
	}
	else if (words[0] == "directory_listing")
	{
		std::cout << "directive: directory_listing\n";
	}
	else if (words[0] == "path")
	{
		std::cout << "directive: path\n";
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

static std::streampos handle_server_directive(std::streampos current_position, std::stack<char> brackets, std::string filepath, std::vector<std::string> words)
{
	if (words[0][0] == '#')
	{
		return current_position;
	}
	else if (words[0] == "host")
	{
		std::cout << "directive: host\n";
	}
	else if (words[0] == "port")
	{
		std::cout << "directive: port\n";
	}
	else if (words[0] == "server_names")
	{
		std::cout << "directive: server_names\n";
	}
	else if (words[0] == "root")
	{
		std::cout << "directive: root\n";
	}
	else if (words[0] == "error_page")
	{
		std::cout << "directive: error_page\n";
	}
	else if (words[0] == "default_error_page")
	{
		std::cout << "directive: default_error_page\n";
	}
	else if (words[0] == "client_max_body_size")
	{
		std::cout << "directive: client_max_body_size\n";
	}
	else if (words[0] == "location")
	{
		std::cout << "LOCATION!\n";
		std::streampos new_position = setup_new_location(brackets, current_position, filepath, words);
		return new_position;
	}
	else
	{
		throw std::runtime_error("Unkown directive: " + words[0]);
	}

	return (current_position);
}

static std::streampos setup_new_server(std::streampos current_position, std::string filepath, std::vector<std::string> current_words)
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
				std::streampos new_position = handle_server_directive(current_position, brackets, filepath, words);
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
					std::streampos current_position = infile.tellg();
					std::streampos new_position = setup_new_server(current_position, filepath, words);
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