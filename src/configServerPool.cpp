/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configServerPool.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felicia <felicia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:49:09 by fkoolhov          #+#    #+#             */
/*   Updated: 2024/04/18 19:43:25 by felicia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configuration.hpp"

static std::streampos handle_serverpool_directive(std::unique_ptr<ServerPool>& serverpool, std::streampos current_position, std::string filepath, std::vector<std::string> words)
{
	if (words[0] == "server")
	{
		std::unique_ptr<Server> server = std::make_unique<Server>();
		std::streampos new_position = configure_server(server, current_position, filepath, words);
		serverpool->addServer(std::move(server));
		return new_position;
	}
	else if (words[0][0] == '#')
		return current_position;
	else
		throw std::runtime_error("Unknown directive: " + words[0]);
	
}

static std::string open_infile(char* filepath_arg, std::ifstream& infile)
{
	std::string user_filepath(filepath_arg);
		
	infile.open(user_filepath);
	if (infile.is_open())
		return user_filepath;
	else
	{
		std::string default_filepath = "./config/default.conf";
		infile.open(default_filepath);
		if (!infile.is_open())
			throw std::runtime_error("Failed to open both the specified file and the default file.");
		return default_filepath;
	}
}

void configure_serverpool(char* filepath_arg)
{
	std::ifstream infile;
	
	try
	{
		std::string filepath = open_infile(filepath_arg, infile);
		std::unique_ptr<ServerPool> serverpool = std::make_unique<ServerPool>();
		std::string line;
		
		while (std::getline(infile, line))
		{
			std::vector<std::string> words = get_words_in_line(line);
			if (words.size() > 0)
			{
				std::streampos current_position = infile.tellg();
				std::streampos new_position = handle_serverpool_directive(serverpool, current_position, filepath, words);
				infile.seekg(new_position);
			}
		}
		infile.close();
		std::cout << *serverpool << std::endl; // for debugging purposes
	}
	catch (const std::exception& exception)
	{
		infile.close();
		std::cerr << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}