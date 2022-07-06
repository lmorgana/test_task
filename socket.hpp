#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "header.h"

class FdHandler
{ //base class
    int fd;

public:
    FdHandler(int a_fd) : fd(a_fd) {}
    virtual ~FdHandler();
    virtual void Handle() = 0;

    int GetFd() const { return fd; }
};

#endif
