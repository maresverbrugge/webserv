/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felicia <felicia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:33:37 by felicia           #+#    #+#             */
/*   Updated: 2024/04/24 11:10:42 by felicia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configuration.hpp"

static void get_path_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error("Invalid number of arguments for path directive.");
	else
		location->setPath(words[1]);
}

static void get_directory_listing_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2 || words[1] != "on")
		location->setDirectoryListing(false);
	else
		location->setDirectoryListing(true);
}

static void get_cgi_extension_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error("Invalid number of arguments for cgi_extension directive.");
	else
		location->setCgiExtension(words[1]);
}

static void get_redirect_link_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error("Invalid number of arguments for redirect directive.");
	else
		location->setRedirectLink(words[1]);
}

static void get_upload_directory_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error("Invalid number of arguments for upload_directory directive.");
	else
		location->setUploadFolder(words[1]);
}

static void get_default_page_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		config_error("Invalid number of arguments for default directive.");
	else
		location->setDefaultPage(words[1]);
}

static void get_allowed_methods_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
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
			config_error("Unknown method: " + words[i]);
	}
	location->setAllowedMethods(allowed_methods);
}

// Checks the current location directive (or comment or invalid directive)
static void handle_location_directive(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words[0][0] == '#')
		return;
	else if (words[0] == "allow_methods")
		get_allowed_methods_from_config(location, words);
	else if (words[0] == "default")
		get_default_page_from_config(location, words);
	else if (words[0] == "upload_directory")
		get_upload_directory_from_config(location, words);
	else if (words[0] == "redirect")
		get_redirect_link_from_config(location, words);
	else if (words[0] == "cgi_extension")
		get_cgi_extension_from_config(location, words);
	else if (words[0] == "directory_listing")
		get_directory_listing_from_config(location, words);
	else if (words[0] == "path")
		get_path_from_config(location, words);
	else
		config_error("Unknown location directive: " + words[0]);
}

static void get_location_name_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid number of arguments for location directive.");
	else if (words[1] == "/")
		location->setIsDefaultLocation(true);
	else
		location->setLocationName(words[1]);
}

// Adds the root folder to location filepaths
static void create_full_location_paths(std::unique_ptr<Location>& location, std::string root_folder)
{
	location->setPath("./" + root_folder + location->getPath());
	if (location->getDefaultPage().length() > 0)
		location->setDefaultPage("./" + root_folder + location->getDefaultPage());
	if (location->getUploadFolder().length() > 0)
		location->setUploadFolder("./" + root_folder + location->getUploadFolder());
	if (location->getRedirectLink().length() > 0)
		location->setRedirectLink("./" + root_folder + location->getRedirectLink());
}

// Reads a location section of the config file and configures a location object
void configure_location(std::unique_ptr<Location>& location, std::ifstream& infile, std::vector<std::string> words, std::string root_folder)
{
	get_location_name_from_config(location, words);
	std::string line;
	std::stack<char> brackets;
	
	while (std::getline(infile, line))
	{
		words = get_words_in_line(line);
		if (words.size() > 0)
		{
			bool found_bracket = check_for_brackets(words, brackets);
			if (!found_bracket)
				handle_location_directive(location, words);
			else if (found_bracket && brackets.size() == 0)
			{
				check_location_config_errors(location);
				create_full_location_paths(location, root_folder);
				return;
			}
		}
	}
	throw std::runtime_error("Location should have closing bracket.");
}