#ifndef SESSION_HPP
#define SESSION_HPP

#include "header.h"
#include "socket.hpp"
#include "server.hpp"
#include "logg.hpp"

class Server;

class PairSession; // has two active session:  1)our server with client,
                                            // 2) our server with forward server


class FdSession : public FdHandler //like one end of the pipe
{
    PairSession *pair_session;

public:
    FdSession(PairSession *a_pair_session, int fd) : FdHandler(fd), pair_session(a_pair_session) {};
    ~FdSession() {};

    void Handle();
    void send(const char *msg);
};

class PairSession //like pipe between client and fw_server
{
    Server      *the_master;
    FdSession   *client; //FdHandler for client
    FdSession   *fw_serv;   //FdHandler for server
    Logg        logg;   //write loggs for all data
    char buff[MAX_LEN + 1];

    FdSession *makeClient(int fd);
    FdSession *makeFwServ();
    int transfer(FdSession *sender, FdSession *destination, int is_sender);
public:
    PairSession() : client(nullptr), fw_serv(nullptr) {};
    PairSession(Server *a_master) : the_master(a_master) {};
    ~PairSession();

    int setConnect(int fd);
    void forwarding(FdSession *session);
    FdSession *getClient() { return (client); };
    FdSession *getFwServer() { return (fw_serv); };
};

#endif
