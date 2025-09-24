#include "Server.hpp"
#include <sstream>
Server::Server() : fd(-1)
{
}
Server::~Server()
{
}
int    Server::GetPort()
{
    return port;
}
int    Server::GetFd()
{
    return fd;
}

void    Server::SetPort(std::string& str)
{
    std::stringstream ss(str);
    int nbr = 0;

    ss >> nbr;
    if(ss.fail())
        throw "Conversion failed for string: " ;
    port = nbr;
    // std::cout << "Port " << port << std::endl;
}


void    Server::SetPassword(std::string password)
{
        this->password = password;
}
std::string   Server::GetPassword()
{
    return password;
}

bool Server::signal = 0;
void     Server::SignalHandel(int signal)
{
    (void) signal;
    std::cout << "Singal Received . " << std::endl;
    Server::signal = true;
}
