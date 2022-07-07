
#include "logg.hpp"

Logg::Logg() : count_of_byte_send(0), count_of_byte_receive(0)
{
    start_time = time(0);
    t_time = localtime(&start_time);
}

Logg::~Logg() {}

void Logg::make_note(char *data, int is_sender) //sender is a flag, if client send message sender=1
{
    file << client_address << "(" << client_port << ") ";
    if (is_sender == 1)
    {
        file << "send to     ";
        count_of_byte_send += strlen(data);
    }
    else
    {
        file << "receive from";
        count_of_byte_receive += strlen(data);
    }
    file << " " << fr_address << "(" << fr_port << "): ";

    print_bytes(file, data, MAX_LEN);
    file << "  ";
    print_char(file, data, MAX_LEN);

    file << std::endl;
}

void Logg::start()
{
    std::string time_file = itostr(t_time->tm_hour) + ":"
                            + itostr(t_time->tm_min) + ":"
                            + itostr(t_time->tm_sec);
    std::string file_name = "log" + itostr(t_time->tm_mday) + "."
                                  + itostr(t_time->tm_mon + 1) + "."
                                  + itostr(t_time->tm_year + 1900) + "-"
                                  + time_file + "-"
                                  + client_address + "(" + client_port + ")-"
                                  + fr_address + "(" + fr_port + ").log";
    std::cout << "path to log of this session: ../logs/" << file_name << "\n" << std::endl;
    file.open("../logs/" + file_name, std::ios_base::out);
    file << "Session start\nTime: " << time_file << std::endl;

    std::string backspaces = "Client";
    backspaces.append(client_address.length() + client_port.length() + 10, ' ');
    backspaces += "Fw_Server";
    backspaces.append(fr_address.length() + fr_port.length() - 4, ' ');
    backspaces += "Bytes";
    backspaces.append(MAX_LEN * 3 - 1, ' ');
    backspaces += "Chars\n";
    file << backspaces;
}

void Logg::end()
{
    time_t  now = time(0);
    tm *l_time = localtime(&now);

    std::string time_file = itostr(l_time->tm_hour) + ":"
                            + itostr(l_time->tm_min) + ":"
                            + itostr(l_time->tm_sec);
    file << "Session close\nTime: " << time_file << "\n";
    file << "Duration: " << std::to_string(now - start_time) << "s\n";
    file << count_of_byte_send << " byte send\n";
    file << count_of_byte_receive << " byte receive" << std::endl;
    file.close();
}

void Logg::setClientPort(unsigned short port) { client_port = std::to_string(ntohs(port)); }

void Logg::setClientAddress(in_addr address)
{
    char *a = inet_ntoa(address);

    client_address = std::string(a);
}

void Logg::setFrPort(unsigned short port) { fr_port = std::to_string(ntohs(port)); }

void Logg::setFrAddress(in_addr address)
{
    char *a = inet_ntoa(address);

    fr_address = std::string(a);
}
