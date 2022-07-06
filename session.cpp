
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
        delete client;
        delete fw_serv;
        std::cout << "*** we can't connect with server, connection will break ***\n" << std::endl;
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

FdSession *PairSession::makeFwServ() //adress and port
{
    int ls, opt, res;
    struct sockaddr_in addr;

    ls = socket(AF_INET, SOCK_STREAM, 0);
    if(ls == -1)
        return (nullptr);
    opt = 1;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(the_master->getFrAddress());
    addr.sin_port = htons(the_master->getFrPort()); // needed to change
    logg.setFrAddress(addr.sin_addr);
    logg.setFrPort(addr.sin_port);
    if (::connect(ls,  (struct sockaddr*) &addr, sizeof(addr)) != 0)
    {
        //something wrong
        close(ls);
        return (nullptr);
    }
    std::cout << "*** we connected with server ***\n" << std::endl;
    return (new FdSession(this, ls));
}

int PairSession::transfer(FdSession *sender, FdSession *destination, const char *name)
{
    int rc = read(sender->GetFd(), buff, MAX_LEN);
    if (rc > 0)
    {
        std::cout << name << " pass: " << buff << std::endl;
        destination->send(buff);
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
    FdSession *sender, *destination;

    if (session == getClient())
        transfer(session, getFwServer(), "Client");
    else if (session == getFwServer())
        transfer(session, getClient(), "FwServer");
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

