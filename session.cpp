
#include "session.hpp"

int PairSession::setConnect(int fd)
{
    client = this->makeClient(fd);
    fw_serv = this->makeFwServ(0);
    if (client && fw_serv)
        return (1);
    else
    {
        if (client)
            delete client;
        if (fw_serv)
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
    if(sd == -1)
        return (nullptr);
    std::cout << "*** client connected with us ***" << std::endl;
    return (new FdSession(this, sd));
}

FdSession *PairSession::makeFwServ(char *address) //adress and port
{
    int ls, opt, res;
    struct sockaddr_in addr;

    ls = socket(AF_INET, SOCK_STREAM, 0);
    if(ls == -1)
        return (nullptr);
    opt = 1;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(6667); // needed to change
    if (::connect(ls,  (struct sockaddr*) &addr, sizeof(addr)) != 0)
    {
        //something wrong
        close(ls);
        return (nullptr);
    }
    std::cout << "*** we connected with server ***\n" << std::endl;
    return (new FdSession(this, ls));
}

void PairSession::forwarding(FdSession *session)
{
    int rc;

    if (session == getClient())
    {
        rc = read(session->GetFd(), buff, MAX_LEN);
        std::cout << "client pass: " << buff << std::endl;
        getFwServer()->send(buff);
        memset(buff, '0', MAX_LEN);
    }
    else if (session == getFwServer())
    {

    }
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

