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

void     Server::SetPort(std::string& str)
{
    port = (std::atoi(str.c_str()));
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
    std::cout << "Welcome to your server! Do you want to give a name to your server? (Y/y): ";
    std::cin >> input;
    if(input.empty())
    {
        std::cout << "No name for your server." << std::endl;
        return ;
    }
    if ( input == "Y" || input == "y")
    {
        std::cout << "Enter the server name: ";
        std::cin >> input;
        this->SetName(input);
    }
    else
        std::cout << "Server will have no name." << std::endl;
}

Client*    Server::getClientByFd(int fd)
{
    for (size_t i = 0; i < clients.size() ; i++)
    {
        if(clients[i].getFd() == fd)
            return  &clients[i];
    }
    return NULL;
}


void         Server::sendToClient(int fd, const std::string& message)
{
    std::string msg = message + "\r\n";

    send(fd , msg.c_str() , msg.length() , 0);
}

void    Server::RemoveClinet(int fd)
{
    for(size_t i = 0 ; i < fds.size() ; i++)
    {
        if (fds[i].fd == fd )
        {
            fds.erase(fds.begin() + i);
            break;
        }
    }
    
    for (size_t i = 0; i < channels.size(); i++)
    {
        channels[i].removeUser(fd);
        channels[i].removeOperator(fd);
        
    }
    
    for (size_t i = 0; i < clients.size(); i++)
    {
        if(clients[i].getFd() == fd)
        {
            clients.erase(clients.begin() + i);
            break;
        }
    }
}

std::string     Server::getCurrentTime()
{
    std::time_t  time = std::time(NULL);
    std::tm  *local  = std::localtime(&time);
    char  buffer[80];
    std::strftime(buffer , sizeof(buffer) , "[%Y-%m-%d %H:%M:%S]" , local);
    return  std::string(buffer);
}

std::vector<Client> Server::getClients()
{
    return clients;
}
