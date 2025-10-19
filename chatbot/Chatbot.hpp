#include <ctime>
#include <cctype>
#include <vector>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include "../server/Client.hpp"

class Chatbot
{
    public:
    static void ident_cmd(std::string arg ,int fd, const std::string &client_name);
    static void help(int fd, const std::string &name);
    static void hour(int fd, const std::string &name);
    static void calculator(int fd, std::string arg , const std::string &name);
    static bool supervisor(int fd, std::string arg , const std::string &name);
    static void sendToDest(std::vector<std::string>  args,Client dest, Client sender);
};