
#include "socket.hpp"

FdHandler::~FdHandler()
{
    close(fd);
}
