#ifndef EVENTSELECTOR_HPP
#define EVENTSELECTOR_HPP

#include "header.h"
#include "socket.hpp"

class EventSelector{
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
