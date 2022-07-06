#ifndef LOGG_HPP
#define LOGG_HPP

#include "header.h"

class Logg
{
private:
    std::ofstream   file;
    tm*             t_time;
    time_t          start_time;
    std::string     fr_port;
    std::string     fr_address;
    std::string     client_port;
    std::string     client_address;
    int             count_of_byte_send;
    int             count_of_byte_receive;

public:
    Logg();
    ~Logg();

    void start();
    void end();
    void make_note(char *data, int receiver);
    void setFrPort(unsigned short port);
    void setFrAddress(in_addr address);
    void setClientPort(unsigned short port);
    void setClientAddress(in_addr address);
};

#endif
