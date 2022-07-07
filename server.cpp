
#include "server.hpp"

Server::Server(EventSelector *sel, int port, char *fr_address, int fr_port)
        : the_selector(sel), fr_port(fr_port), fr_address(fr_address)
{
    listener = this->make_listener(port);
    if (listener)
        the_selector->Add((FdHandler *) (listener));
}

Server::~Server()
{
    the_selector->Remove(listener);

    while(first) {
        item *tmp = first;
        first = first->next;
        the_selector->Remove(tmp->s->getClient());
        the_selector->Remove(tmp->s->getFwServer());
        delete tmp->s;
        delete tmp;
    }
}

FdListener *Server::make_listener(int port)
{
    int fd, opt, res;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
        return (nullptr);
    opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    res = bind(fd, (struct sockaddr*) &addr, sizeof(addr));
    if(res == -1)
        return (nullptr);
    res = listen(fd, BACKLOG_LISTEN);
    if(res == -1)
        return (nullptr);
    return (new FdListener(fd, this));
}

int Server::Start()
{
    if (!listener)
    {
        std::cout << "something wrong" << std::endl;
        return (0);
    }
    std::cout << "Hello, proxy-server is here, i'm forwarding data to " << fr_address << "(" << fr_port << ")" << std::endl;
    the_selector->Run();
    return (1);
}

void Server::RemovePairSession(PairSession *ps)
{
    the_selector->Remove(ps->getClient());
    the_selector->Remove(ps->getFwServer());
    item **p;
    for(p = &first; *p; p = &((*p)->next)) {
        if((*p)->s == ps) {
            item *tmp = *p;
            *p = tmp->next;
            delete tmp->s;
            delete tmp;
            return;
        }
    }
}

void Server::AppendPairSession(PairSession *ps)
{
    item *p = new item;
    p->next = first;
    p->s = ps;
    first = p;
    the_selector->Add(ps->getFwServer());
    the_selector->Add(ps->getClient());
}

void Server::JoinPair(int fd)
{
    PairSession *ps = new PairSession(this);

    if (ps->setConnect(fd) != 0)
        this->AppendPairSession(ps);
    else
        delete ps;
}
