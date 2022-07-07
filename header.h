#ifndef HEADER_H
#define HEADER_H

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/un.h>
#include <fcntl.h>
#include <map>

#include <unistd.h>

#include <poll.h>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include <fstream>
#include <ctime>

#define MAX_LEN 8
#define BACKLOG_LISTEN 16

std::string itostr(int time);
void print_bytes(std::ofstream &file, char *data, int len);
void print_char(std::ofstream &file, char *data, int len);

#endif
