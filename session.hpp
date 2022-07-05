#ifndef SESSION_HPP
#define SESSION_HPP

#include "header.h"
#include "socket.hpp"

#define MAX_LEN 8

class PairSession; // has two active session:  1)our server with client,
                                            // 2) our server with forward server


class FdSession : public FdHandler
{
    PairSession *pair_session;

public:
    FdSession(PairSession *a_pair_session, int fd) : pair_session(a_pair_session), FdHandler(fd) {};
    ~FdSession() {};

    void Handle();
    void send(const char *msg);
};

class PairSession
{
    FdSession   *client; //FdHandler for client
    FdSession   *fw_serv;   //FdHandler for server
//    logg        logg;   //write loggs for all data
    char buff[MAX_LEN];

    FdSession *makeClient(int fd);
    FdSession *makeFwServ(char *address);

public:
    PairSession() : client(nullptr), fw_serv(nullptr) {};
    PairSession(FdSession *a_client, FdSession *a_fw_serv) : client(a_client), fw_serv(a_fw_serv) {};
    ~PairSession() {};

    int setConnect(int fd);
    void forwarding(FdSession *session);
    FdSession *getClient() { return (client); };
    FdSession *getFwServer() { return (fw_serv); };
};

#endif
