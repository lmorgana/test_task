
#include "server.hpp"
#include "session.hpp"

Server::Server(EventSelector *sel, int fd, char *pass, int fr_port)
		: FdHandler(fd, true), the_selector(sel), first(0), fr_port(fr_port)
{
	the_selector->Add(this);
}

Server::~Server()
{
	while(first) {
		item *tmp = first;
		first = first->next;
		the_selector->Remove(tmp->s);
		delete tmp->s;
		delete tmp;
	}
	the_selector->Remove(this);
}

Server *Server::Start(EventSelector *sel, int port, char *address, int fr_port)
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
	addr.sin_port = htons(port);
	res = bind(ls, (struct sockaddr*) &addr, sizeof(addr));
	if(res == -1)
		return (nullptr);

	res = listen(ls, qlen_for_listen);
	if(res == -1)
		return (nullptr);
	std::cout << "Hello, proxy-server is here, we forwarding data to " << address << ":" << port << std::endl;
	return new Server(sel, ls, address, fr_port);
}

void Server::RemoveSession(Session *s, const char *msg)
{
	std::cout << msg << std::endl;
	the_selector->Remove(s);
	item **p;
	for(p = &first; *p; p = &((*p)->next)) {
		if((*p)->s == s) {
			item *tmp = *p;
			*p = tmp->next;
			delete tmp->s;
			delete tmp;
			return;
		}
	}
}

void Server::appendSession(Session *session)
{
    item *p = new item;
    p->next = first;
    p->s = session;
    first = p;

    the_selector->Add(p->s);
}

void Server::Handle(bool r, bool w)
{
	if(!r)
		return;
	int sd;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	sd = accept(GetFd(), (struct sockaddr*) &addr, &len);
	if(sd == -1)
		return;
    Client *client = new Client(this, sd);
    std::cout << "*** client connected with us ***" << std::endl;
    Session *server_session = client->connect();
    if (server_session)
    {
        this->appendSession(client);
        this->appendSession(server_session);
        client->set_pair(server_session);
        server_session->set_pair(client);
        std::cout << "*** we connected with server ***\n" << std::endl;
    }
    else
    {
        delete client;
        std::cout << "*** we can't connect with server, connection will break ***\n" << std::endl;

    }
}
