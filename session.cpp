
#include "session.hpp"

PairSession::~PairSession()
{
    delete client;
    delete fw_serv;
}

int PairSession::setConnect(int fd)
{
    client = this->makeClient(fd);
    fw_serv = this->makeFwServ();
    if (client && fw_serv)
    {
        logg.start();
        return (1);
    }
    else
    {
        if (client)
            client->send("error: can't connect with forwarding server");
        std::cout << "*** we can't connect with server, connection will break ***" << std::endl;
        return (0);
    }
}

FdSession *PairSession::makeClient(int fd)
{
    int sd;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    sd = accept(fd, (struct sockaddr*) &addr, &len);
    logg.setClientAddress(addr.sin_addr);
    logg.setClientPort(addr.sin_port);
    if(sd == -1)
        return (nullptr);
    std::cout << "*** client connected with us ***" << std::endl;
    return (new FdSession(this, sd));
}

FdSession *PairSession::makeFwServ()
{
    int ls, opt;
    struct sockaddr_in addr;

    ls = socket(AF_INET, SOCK_STREAM, 0);
    if(ls == -1)
        return (nullptr);
    opt = 1;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(the_master->getFrAddress());
    addr.sin_port = htons(the_master->getFrPort());
    logg.setFrAddress(addr.sin_addr);
    logg.setFrPort(addr.sin_port);
    if (::connect(ls,  (struct sockaddr*) &addr, sizeof(addr)) != 0)
    {
        std::cout << "*** we can't connected with server ***" << std::endl;
        close(ls);
        return (nullptr);
    }
    std::cout << "*** user connected with fw_server ***" << std::endl;
    return (new FdSession(this, ls));
}

int PairSession::transfer(FdSession *sender, FdSession *destination, int is_sender)
{
    int rc = read(sender->GetFd(), buff, MAX_LEN);
    if (rc > 0)
    {
        destination->send(buff);
        logg.make_note(buff, is_sender);
    }
    else if (rc == 0)
    {
        the_master->RemovePairSession(this);
        std::cout << "*** user disconnected ***" << std::endl;
        return (0);
    }
    memset(buff, '\0', MAX_LEN);
    return (rc);
}

void PairSession::forwarding(FdSession *session)
{
    if (session == getClient())
        transfer(session, getFwServer(), 1); // 1 - client >> sender
    else if (session == getFwServer())
        transfer(session, getClient(), 0); // 0 - fwServer >> receive
}

void FdSession::Handle()
{
    pair_session->forwarding(this);
};

void FdSession::send(const char *msg)
{
    if (msg)
        write(GetFd(), msg, strlen(msg));
};

