#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "header.h"

class Server;

class FdHandler
{ //base class
    int fd;

public:
    FdHandler(int a_fd) : fd(a_fd) {}
    virtual ~FdHandler();
    virtual void Handle() = 0;

    int GetFd() const { return fd; }
};

class EventSelector {
    typedef std::vector<FdHandler*>::iterator iterator_handler;
    typedef std::vector<pollfd>::iterator iterator_pollfd;

    std::vector<FdHandler*> fd_array;
    std::vector<pollfd> poll_array;

    int max_fd;
    bool quit_flag;
public:
    EventSelector() : fd_array(), quit_flag(false) {}
    ~EventSelector() {};

    void Add(FdHandler *h);
    bool Remove(FdHandler *h);

    void BreakLoop() { quit_flag = true; }
    void Run();

private:
    int	search_fd_poolfd(int fd);
};

#endif
