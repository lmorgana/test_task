#ifndef SERVER_HPP
#define SERVER_HPP

#include "header.hpp"
#include "sockets.hpp"
#include "session.hpp"

class Session;

class Server : public FdHandler {
	EventSelector	*the_selector;
	struct item {
		Session *s;
		item *next;
	};
	item *first;
    int  fr_port;

	Server(EventSelector *sel, int fd, char *pass, int fr_port);
public:
	~Server();

	static Server *Start(EventSelector *sel, int port, char *address, int fr_port);

    void appendSession(Session *session);
	void RemoveSession(Session *s, const char *msg);

private:
	virtual void Handle(bool r, bool w);
};

#endif
