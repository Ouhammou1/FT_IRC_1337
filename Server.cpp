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

