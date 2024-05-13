#include "RequestHandler.hpp"
#include <filesystem>


void RequestHandler::handleDeleteRequest()
{
    std::filesystem::path filePath(getAbsolutePath(_request.getPath()));

    if (std::filesystem::exists(filePath))
    {
        if (std::filesystem::remove_all(filePath))
            setStatusCode(NO_CONTENT);
        else
            throw (INTERNAL_SERVER_ERROR);
    }
    else
        throw (NOT_FOUND);
}
