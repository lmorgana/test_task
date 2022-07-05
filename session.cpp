
#include "session.hpp"
#include <cstring>


Session::Session(Server *a_master, int fd)
		: FdHandler(fd, true), stat(READING), buf_used(0), ignoring(false),
		  name(0), the_master(a_master), pair(nullptr)
		  {}

Session::~Session()
{
	if(name)
		delete[] name;
}

int get_line(int fd, char *buffer, int len_buffer)
{
	char ch;
	int i = 0;
	int rc;

	while ((rc = read(fd, &ch, 1)) > 0)
	{
		if (i < len_buffer && ch != '\n')
			buffer[i] = ch;
		else if (ch == '\n')
		{
			buffer[i] = ch;
			break;
		}
		i++;
	}
	return (rc);
}

void Session::Handle(bool r, bool w)
{
//	std::vector<struct returnRes> *result = new std::vector<struct returnRes>;
    std::string *result = new std::string("dsf");
	if (r)
	{
		int rc = get_line(GetFd(), buffer, sizeof(buffer));
		if (rc > 0)
		{
//			std::cout << "buffer: \"" << buffer << "\"" << "strlen" << strlen(buffer) << std::endl;
//			result = checkData(this, buffer, the_master->getBook(), result);
//			the_master->send_msg(result);
            this->get_pair()->send(buffer);
		}
		else if (rc == 0)
		{
			the_master->RemoveSession(this, EXIT_MSG);
			delete result;
			return ;
		}
		else
		{
			the_master->RemoveSession(this, "*** server close session ***");
			delete result;
			return ;
		}
		bzero(buffer, strlen(buffer));
		delete result;
	}
}

void Session::set_pair(Session *pr)
{
    pair = pr;
}

Session *Session::get_pair()
{
    return (pair);
}

void Session::send(const char *msg)
{
	if (msg)
		write(GetFd(), msg, strlen(msg));
}

Client::Client(Server *a_master, int fd)
        : Session(a_master, fd)
{}

Session *Client::connect(void)
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
    Session *session = new Session(the_master, ls);
    return (session);
};

