#include "Server.hpp"
#include <sstream>
Server::Server() : fd(-1) , name("")
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


void      Server::CloseServer()
{
    for (size_t i = 0; i < fds.size(); i++)
    {
        if (fds[i].fd != fd && fds[i].fd != -1)
        {
            close(fds[i].fd);
        }
    }
    fds.clear();
    clients.clear();
    std::cout << "Server closed successfully."  << std::endl;
}

void    Server::SetName(std::string name)
{
    this->name = name;
}

std::string     Server::GetName()
{
    return name;
}
void     Server::setNameServer()
{
    std::string input ;
    std::cout << "Welcome to your server! Do you want to give a name to your server? (y/n): ";
    std::cin >> input;
    if(input.empty())
    {
        std::cout << "No name for your server." << std::endl;
        return ;
    }
    if ( input == "Y" | input == "y")
    {
        std::cout << "Enter the server name: ";
        std::cin >> input;
        this->SetName(input);
    }
    else
        std::cout << "Server will have no name." << std::endl;
}

