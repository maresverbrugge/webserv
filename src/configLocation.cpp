/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felicia <felicia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:33:37 by felicia           #+#    #+#             */
/*   Updated: 2024/04/18 19:15:02 by felicia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configuration.hpp"

static void get_path_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid number of arguments for path directive.");

	location->setPath(words[1]);
}

static void get_directory_listing_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2 || words[1] == "off")
		location->setDirectoryListing(false);
	if (words[1] == "on")
		location->setDirectoryListing(true);
	else
		throw std::runtime_error("Invalid argument for directory_listing directive.");
}

static void get_cgi_extension_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid number of arguments for cgi_extension directive.");

	location->setCgiExtension(words[1]);
}

static void get_redirect_link_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid number of arguments for redirect directive.");

	location->setRedirectLink(words[1]);
}

static void get_upload_directory_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid number of arguments for upload_directory directive.");

	location->setUploadFolder(words[1]);
}

static void get_default_page_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid number of arguments for default directive.");

	location->setDefaultPage(words[1]);
}

static void get_allowed_methods_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	std::array<bool, 3> allowed_methods = {false, false, false};

	for (size_t i = 1; i < words.size(); i++)
	{
		if (words[i] == "GET")
			allowed_methods[GET] = true;
		else if (words[i] == "POST")
			allowed_methods[POST] = true;
		else if (words[i] == "DELETE")
			allowed_methods[DELETE] = true;
		else
			throw std::runtime_error("Unkown method: " + words[i]);
	}
	location->setAllowedMethods(allowed_methods);
}

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
		throw std::runtime_error("Unkown directive: " + words[0]);
}

static void get_location_name_from_config(std::unique_ptr<Location>& location, std::vector<std::string> words)
{
	if (words.size() < 2)
		throw std::runtime_error("Invalid number of arguments for location directive.");

	location->setLocationName(words[1]);
}

std::streampos configure_location(std::unique_ptr<Location>& location, std::stack<char> brackets, std::streampos current_position, std::string filepath, std::vector<std::string> current_words)
{
	get_location_name_from_config(location, current_words);

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
				handle_location_directive(location, words);	
			}
		}
	}

	infile.close();
	throw std::runtime_error("No location closing bracket.");
}