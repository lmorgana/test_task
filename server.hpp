#ifndef SERVER_HPP
#define SERVER_HPP

#include "header.h"
#include "EventSelector.hpp"
#include "session.hpp"
#include "FdListener.hpp"

class PairSession;
class FdListener;

class Server {
    EventSelector	*the_selector; //contains and listening all ports(sockets) to read
    FdListener      *listener;  //socket to listening input connection
    int             fr_port;    //port of fw_server(forward to server)
    char            *fr_address; //address of fw_server

    struct item {
        PairSession *s;
        item *next;
    };
    item *first;

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
