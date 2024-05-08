/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configlocation_info.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:33:37 by felicia           #+#    #+#             */
/*   Updated: 2024/05/08 13:07:00 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configuration.hpp"

static void get_path_from_config(location_t& location_info, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for path directive.");
	else
		location_info.path = words[1];
}

static void get_directory_listing_from_config(location_t& location_info, std::vector<std::string> words)
{
	if (words.size() < 2 || words[1] != "on")
		location_info.directory_listing = false;
	else
		location_info.directory_listing = true;
}

static void get_cgi_extension_from_config(location_t& location_info, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for cgi_extension directive.");
	else
		location_info.cgi_extension = words[1];
}

static void get_redirect_link_from_config(location_t& location_info, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for redirect directive.");
	else
		location_info.redirect_link = words[1];
}

static void get_upload_directory_from_config(location_t& location_info, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for upload_directory directive.");
	else
		location_info.upload_folder = words[1];
}

static void get_default_page_from_config(location_t& location_info, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error_message("Invalid number of arguments for default directive.");
	else
		location_info.default_page = words[1];
}

static void get_allowed_methods_from_config(location_t& location_info, std::vector<std::string> words)
{
	std::array<bool, 3> allowed_methods = {false, false, false};

	for (size_t i = 1; i < words.size() && words[i][0] != '#'; i++)
	{
		if (words[i] == "GET")
			allowed_methods[GET] = true;
		else if (words[i] == "POST")
			allowed_methods[POST] = true;
		else if (words[i] == "DELETE")
			allowed_methods[DELETE] = true;
		else
			config_error_message("Unknown method: " + words[i]);
	}
	location_info.allowed_methods = allowed_methods;
}

// Checks the current location directive (or comment or invalid directive)
static void handle_location_directive(location_t& location_info, std::vector<std::string> words)
{
	if (words[0][0] == '#')
		return;
	else if (words[0] == "allow_methods")
		get_allowed_methods_from_config(location_info, words);
	else if (words[0] == "default")
		get_default_page_from_config(location_info, words);
	else if (words[0] == "upload_directory")
		get_upload_directory_from_config(location_info, words);
	else if (words[0] == "redirect")
		get_redirect_link_from_config(location_info, words);
	else if (words[0] == "cgi_extension")
		get_cgi_extension_from_config(location_info, words);
	else if (words[0] == "directory_listing")
		get_directory_listing_from_config(location_info, words);
	else if (words[0] == "path")
		get_path_from_config(location_info, words);
	else
		config_error_message("Unknown location directive: " + words[0]);
}

// Adds the root folder to location filepaths
static void create_full_location_paths(location_t& location_info, std::string root_folder)
{
	location_info.path = "./" + root_folder + location_info.path;
	if (location_info.default_page.length() > 0)
		location_info.default_page = "./" + root_folder + location_info.default_page;
	if (location_info.upload_folder.length() > 0)
		location_info.upload_folder = "./" + root_folder + location_info.upload_folder;
}

// Reads a location section of the config file and configures a location object
int configure_location(location_t& location_info, std::ifstream& infile, std::vector<std::string> words, std::string root_folder, bool is_default_location)
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
				handle_location_directive(location_info, words);
			else if (found_bracket && brackets.size() == 0)
			{
				int config_error = check_location_config_errors(location_info, is_default_location);
				create_full_location_paths(location_info, root_folder);
				return config_error;
			}
		}
	}
	throw std::runtime_error("Location should have closing bracket.");
}