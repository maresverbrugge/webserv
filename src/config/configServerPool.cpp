/* ************************************************************************* */
/*      ##       ##      ## ##       ##      ## ##       ##      ##          */
/*       ##     ####    ##   ##     ####    ##   ##     ####    ##           */
/*        ##  ##   ##  ##     ##  ##   ##  ##     ##  ##   ##  ##            */
/*         ####     ####       ####     ####       ####     ####             */
/*          ##       ##         ##       ##         ##       ##              */
/*                                                                           */
/*           WONDERFUL            WEBSERV           WONDERTEAM               */
/*                                                                           */
/*      FELICIA KOOLHOVEN      FLEN HUISMAN       MARES VERBRUGGE            */
/*          fkoolhov             fhuisman             mverbrug               */
/*                                                                           */
/*          Codam Coding College        part of 42 network                   */
/*                            April - May 2024                               */
/* ************************************************************************* */

#include "configuration.hpp"

static void initialize_server_info(server_t& server_info)
{
	server_info.port = 8080;
	server_info.host = "0.0.0.0";
	server_info.server_names = std::vector<std::string>();
	server_info.root_folder = "";
	server_info.custom_error_pages = std::map<short, std::string>();
	server_info.client_max_body_size = 1024 * 1024;
	server_info.locations = std::vector<std::unique_ptr<Location>>();
	server_info.default_location = nullptr;
}

static bool port_already_occupied(ServerPool& serverpool, int port_to_add)
{
	const std::vector<std::unique_ptr<Server>>& servers = serverpool.getServers();
	for (const std::unique_ptr<Server>& server : servers)
	{
		if (server->getPort() == port_to_add)
		{
			config_error_message("Port " + std::to_string(port_to_add) + " already occupied by another server.");
			return true;
		}
	}
	return false;
}

// Checks if line in config file is empty, comment, declares new server, or is invalid
static void handle_serverpool_directive(ServerPool& serverpool, std::ifstream& infile, std::vector<std::string> words)
{
	if (words[0][0] == '#')
		return;
	else if (words[0][0] != '#' && words[0] == "server")
	{
		server_t server_info;

		initialize_server_info(server_info);
		int config_error = configure_server(server_info, infile, words);
		if (config_error == EXIT_SUCCESS && !port_already_occupied(serverpool, server_info.port))
		{
			serverpool.addServer(std::make_unique<Server>(server_info.port,
															server_info.host, 
															server_info.server_names, 
															server_info.root_folder, 
															server_info.custom_error_pages, 
															server_info.client_max_body_size, 
															std::move(server_info.locations), 
															std::move(server_info.default_location),
															serverpool));
		}
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
ServerPool& configure_serverpool(char* filepath_arg)
{
	std::ifstream infile;
	
	try
	{
		open_infile(filepath_arg, infile);
		// std::unique_ptr<ServerPool> serverpool = std::make_unique<ServerPool>();
		ServerPool& serverpool = ServerPool::getInstance();
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