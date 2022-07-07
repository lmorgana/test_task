
#include "header.h"
#include "server.hpp"
#include "session.hpp"

#define MAX_PORT 49151
#define MIN_PORT 1023

bool is_digit(char *str)
{
    if (!str)
        return (false);
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        if (isdigit(!str[i]))
            return (false);
    }
    return (true);
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cout << "Wrong number of arguments\n(listening port, address, port)" << std::endl;
        return (1);
    }
    if (!is_digit(argv[1]) && !is_digit(argv[3]))
    {
        std::cout << "Ports must be number" << std::endl;
        return (1);
    }
    int listening_port = atoi(argv[1]);
    int fr_port = atoi(argv[3]);
    if (listening_port <= MIN_PORT || listening_port >= MAX_PORT)
    {
        std::cout << "Wrong port number: port must be >= " << MIN_PORT << " and <= " << MAX_PORT << std::endl;
        return (1);
    }
    EventSelector *selector = new EventSelector;
    Server *serv = new Server(selector, listening_port, argv[2], fr_port);
    serv->Start();
    return 0;
}