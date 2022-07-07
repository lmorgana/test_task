#ifndef FDLISTENER_HPP
#define FDLISTENER_HPP

#include "server.hpp"
#include "socket.hpp"

class Server;

class FdListener : public FdHandler    //class to listen input port for input connection
{
    Server *the_master;

public:
    FdListener(int fd, Server *a_master) : FdHandler(fd), the_master(a_master) {};
    ~FdListener() {};
    void Handle();
};

#endif
