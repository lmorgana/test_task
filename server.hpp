#ifndef SERVER_HPP
#define SERVER_HPP

#include "header.h"
#include "EventSelector.hpp"
#include "session.hpp"
#include "FdListener.hpp"

class PairSession;
class FdListener;

class Server {
    EventSelector	*the_selector;
    FdListener      *listener;
    int             fr_port;
    char            *fr_address;

    struct item {
        PairSession *s;
        item *next;
    };
    item *first;

//    Server() {};
//    Server(Server &other) {};
//    Server &operator=(Server &other) {};
    FdListener *make_listener(int port);

public:
    Server(EventSelector *sel, int port, char *fr_address, int fr_port); //forward address and forward port
    ~Server();

    int Start();

    void JoinPair(int fd);
    void AppendPairSession(PairSession *session);
    void RemovePairSession(PairSession *ps);
    int getFrPort() { return (fr_port); };
    char *getFrAddress() { return (fr_address); };
};

#endif
