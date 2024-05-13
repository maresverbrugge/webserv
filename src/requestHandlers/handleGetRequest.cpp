/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGetRequest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 17:28:08 by fhuisman          #+#    #+#             */
/*   Updated: 2024/05/13 13:20:13 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include <dirent.h> //opendir()
#include <filesystem> //is_directory()

std::string RequestHandler::getAbsolutePath(std::string path)
{
    return (_location.getPath() + path.substr(path.find(_location.getLocationName()) + _location.getLocationName().size()));
}

std::string RequestHandler::constructBodyFromDirectory(std::string path)
{
    std::string body;

    if (!_location.getDirectoryListing())
    {
        std::string defaultPage = _location.getDefaultPage();
        if (defaultPage == "")
            throw (FORBIDDEN);
        addHeader("Content-Type", "text/html");
        return (constructBodyFromFile(defaultPage));
    }
    DIR* dir_stream = opendir(path.c_str());
    if (!dir_stream)
        throw (NOT_FOUND);
    struct dirent *dirent;
    body = "<!DOCTYPE html>\r\n<html lang=\"en\">\r\n<head>\r\n\t<meta charset=\"UTF-8\">\r\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n\t<title>Directory Listing</title>\r\n</head>\r\n<body>\r\n\t<h1>Directory Listing</h1>\r\n\t<p>The page you requested is a directory</p>\r\n\t<ul>\r\n";
    while ((dirent = readdir(dir_stream)))
    {
        if (dirent->d_name[0] != '.')
            body += "\t\t<li><a href=\"" + path + dirent->d_name + "\">" + dirent->d_name + "</a></li>\r\n";
    }
    body += "\t</ul>\r\n</body>\r\n</html>";
    closedir(dir_stream);
    addHeader("Content-Type", "text/html");
    return (body);
}

std::string RequestHandler::constructBody(std::string path)
{
    std::string body;
    std::ifstream file;
    
    path = getAbsolutePath(path);
    if (std::filesystem::is_directory(path))
        return (constructBodyFromDirectory(path));
    body = constructBodyFromFile(path);
    addHeader("Content-Type", getContentType(path));
    return (body);
}

void RequestHandler::handleGetRequest()
{
    setBody(constructBody(_request.getPath()));
    if (_body.size() != 0)
        addHeader("Content-Length", std::to_string(_body.size()));
}