/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   configServerPool.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: felicia <felicia@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/16 14:49:09 by fkoolhov      #+#    #+#                 */
/*   Updated: 2024/05/07 15:14:09 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "configuration.hpp"

// Checks if line in config file is empty, comment, declares new server, or is invalid
static void handle_serverpool_directive(std::unique_ptr<ServerPool>& serverpool, std::ifstream& infile, std::vector<std::string> words)
{
	if (words[0][0] == '#')
		return;
	else if (words[0][0] != '#' && words[0] == "server")
	{
		std::unique_ptr<Server> server = std::make_unique<Server>();
		int config_error = configure_server(server, infile, words);
		if (config_error == EXIT_SUCCESS)
			serverpool->addServer(std::move(server));
	}
	else
		config_error_message("Unknown serverpool directive: " + words[0]);
}

// Opens the config file specified by the user or otherwise the default config file
static void open_infile(char* filepath_arg, std::ifstream& infile)
{
	std::string user_filepath(filepath_arg);
	infile.open(user_filepath);
	if (!infile.is_open())
	{
		infile.open(DEFAULT_CONFIG);
		if (!infile.is_open())
			throw std::runtime_error("Failed to open both the specified file and the default file.");
	}
}

// Reads the config file and creates a ServerPool of Servers
std::unique_ptr<ServerPool> configure_serverpool(char* filepath_arg)
{
	std::ifstream infile;
	
	try
	{
		open_infile(filepath_arg, infile);
		std::unique_ptr<ServerPool> serverpool = std::make_unique<ServerPool>();
		std::string line;
		while (std::getline(infile, line)) // make everything lowercase?
		{
			std::vector<std::string> words = get_words_in_line(line);
			if (words.size() > 0)
				handle_serverpool_directive(serverpool, infile, words);
		}
		check_serverpool_config_errors(serverpool);
		infile.close();
		return serverpool;
	}
	catch (const std::exception& exception)
	{
		if (infile.is_open())
			infile.close();
		std::cerr << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}