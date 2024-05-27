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

static void get_client_max_body_size_from_config(server_t& server_info, std::vector<std::string> words)
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
			long long max_body_size = std::stoll(max_body_size_str) * multiplier;
			server_info.client_max_body_size = max_body_size;
		}
		catch (const std::exception& exception)
		{
			config_error_message("Invalid client_max_body_size: " + words[1] + ": " + exception.what());
		}
	}
}

static void get_custom_error_page_from_config(server_t& server_info, std::vector<std::string> words)
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
			server_info.custom_error_pages[static_cast<short>(std::stoi(words[j]))] = error_page;
	}
}

static void get_root_folder_from_config(server_t& server_info, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for root directive.");
	else
		server_info.root_folder = words[1];
}

static void get_server_names_from_config(server_t& server_info, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for server_names directive.");
	else
	{
		for (size_t i = 1; i < words.size(); i++)
		{
			if (words[i][0] == '#')
				break;
			server_info.server_names.push_back(words[i]);
		}
	}
}

static void get_host_from_config(server_t& server_info, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for host directive.");
	else
		server_info.host = words[1];
}

static void get_port_from_config(server_t& server_info, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for port directive.");
	else
	{
		try
		{
			server_info.port = std::stoi(words[1]);
		}
		catch (const std::exception& exception)
		{
			config_error_message("Invalid port number: " + words[1] + ": " + exception.what());
		}
	}
}

static bool get_location_name_from_config(location_t& location_info, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for location directive.");
	else if (words[1] == "/")
		return true;
	else
		location_info.location_name = words[1];
	return false;
}

static void initialize_location_info(location_t& location_info)
{
	location_info.location_name = "";
	location_info.allowed_methods = {false, false, false};
	location_info.path = "";
	location_info.default_page = "";
	location_info.redirect_link = "";
	location_info.cgi_extension = "";
	location_info.upload_folder = "";
	location_info.directory_listing = false;
}

static void create_new_location_object(server_t& server_info, std::ifstream& infile, std::vector<std::string> words)
{
	location_t location_info;

	initialize_location_info(location_info);
	bool is_default_location = get_location_name_from_config(location_info, words);
	if (is_default_location && server_info.default_location != nullptr)
		throw std::runtime_error("Server can have only one default location.");
	int config_error = configure_location(location_info, infile, words, server_info.root_folder, is_default_location);
	
	if (config_error == EXIT_SUCCESS)
	{
		std::unique_ptr<Location> location = std::make_unique<Location>(location_info.location_name,
																		location_info.allowed_methods,
																		location_info.path,
																		location_info.default_page,
																		location_info.redirect_link,
																		location_info.cgi_extension,
																		location_info.upload_folder,
																		location_info.directory_listing);
		if (is_default_location)
			server_info.default_location = std::move(location);
		else
			server_info.locations.push_back(std::move(location));
	}
}

// Checks the current server directive (or comment or invalid directive)
static void handle_server_directive(server_t& server_info, std::ifstream& infile, std::vector<std::string> words)
{
	if (words[0][0] == '#')
		return;
	else if (words[0] == "port")
		get_port_from_config(server_info, words);
	else if (words[0] == "host")
		get_host_from_config(server_info, words);
	else if (words[0] == "server_names")
		get_server_names_from_config(server_info, words);
	else if (words[0] == "root")
		get_root_folder_from_config(server_info, words);
	else if (words[0] == "error_page")
		get_custom_error_page_from_config(server_info, words);
	else if (words[0] == "client_max_body_size")
		get_client_max_body_size_from_config(server_info, words);
	else if (words[0] == "location")
		create_new_location_object(server_info, infile, words);
	else
		config_error_message("Unknown server directive: " + words[0]);
}

// Adds the root folder to server filepaths
static void create_full_server_paths(server_t& server_info)
{
	server_info.root_folder = "./" + server_info.root_folder;
	std::map<short, std::string> custom_error_pages = server_info.custom_error_pages;
	for (auto it = custom_error_pages.begin(); it != custom_error_pages.end(); it++)
		server_info.custom_error_pages[it->first] = server_info.root_folder + it->second;
}

// Reads the server section of the config file and configures a server object
int configure_server(server_t& server_info, std::ifstream& infile, std::vector<std::string> words)
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
				handle_server_directive(server_info, infile, words);
			else if (found_bracket && brackets.size() == 0)
			{
				int config_error = check_server_config_errors(server_info);
				create_full_server_paths(server_info);
				return config_error;
			}
		}
	}
	throw std::runtime_error("Server should have closing bracket.");
}