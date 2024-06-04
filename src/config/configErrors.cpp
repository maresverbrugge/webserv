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

void check_serverpool_config_errors(ServerPool& serverpool)
{
	if (serverpool.getServers().size() == 0)
		throw std::runtime_error("ServerPool must contain at least one server.");
}

int check_server_config_errors(server_t& server_info)
{
	if (server_info.port < 1 || server_info.port > 65535)
    	config_error_message("Server port number must be between 1 and 65535.");
	else if (server_info.host.length() == 0)
		config_error_message("Server must have host.");
	else if (server_info.root_folder.length() == 0)
		config_error_message("Server must have root folder.");
	else
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}

int check_location_config_errors(location_t& location_info, bool is_default_location)
{
	if (location_info.location_name.length() == 0 && !is_default_location)
		config_error_message("Server location must have location name.");
	else if (location_info.path.length() == 0)
		config_error_message("Server location must have path.");
	else
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}