
#include "header.h"

char get_leter(int num)
{
    if (num < 10)
        return (num + 48);
    else if (num >= 10 && num <= 15)
        return (num + 55);
    return (0);
}

std::string ch_to_unch(int data)
{
    char result[3];

    result[0] = get_leter(data / 16);
    result[1] = get_leter(data % 16);
    result[2] = '\0';
    return (std::string(result));
}

void print_bytes(std::ofstream &file, char *data, int len)
{
    file << "|";
    for (int i = 0; i < len; i++)
    {
        if (isprint(data[i]))
            file << ch_to_unch(data[i]) << " ";
        else
            file << "   ";
    }
    file << "|";
}

void print_char(std::ofstream &file, char *data, int len)
{
    file << "|";
    for (int i = 0; i < len; i++)
    {
        if (isprint(data[i]))
            file << data[i] << " ";
        else
            file << "  ";
    }
    file << "|";
}

std::string itostr(int time)
{
    std::string str = std::to_string(time);
    if (str.length() < 2)
        str = "0" + str;
    return (str);
}
