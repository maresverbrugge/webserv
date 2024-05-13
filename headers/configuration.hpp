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

#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include "ServerPool.hpp"
# include "Server.hpp"
# include "Location.hpp"

typedef struct location_s
{
	std::string location_name;
	std::array<bool, 3> allowed_methods;
	std::string path;
	std::string default_page;
	std::string redirect_link;
	std::string cgi_extension;
	std::string upload_folder;
	bool directory_listing;
} location_t;

typedef struct server_s 
{
    int port;
    std::string host;
    std::vector<std::string> server_names;
    std::string root_folder;
    std::map<short, std::string> custom_error_pages;
    unsigned long long client_max_body_size;
    std::vector<std::unique_ptr<Location>> locations;
    std::unique_ptr<Location> default_location;
} server_t;

// configServerPool.cpp
std::unique_ptr<ServerPool> configure_serverpool(char* filepath);

// configServer.cpp
int configure_server(server_t& server_struct, std::ifstream& infile, std::vector<std::string> words);

// configLocation.cpp
int configure_location(location_t& location, std::ifstream& infile, std::vector<std::string> words, std::string root_folder, bool is_default_location);

// configUtils.cpp
std::vector<std::string> get_words_in_line(std::string line);
bool check_for_brackets(std::vector<std::string> words, std::stack<char>& brackets);
void config_error_message(std::string message);

// configErrors.cpp
int check_location_config_errors(location_t& location, bool is_default_location);
int check_server_config_errors(server_t& server);
void check_serverpool_config_errors(std::unique_ptr<ServerPool>& serverpool);

#endif