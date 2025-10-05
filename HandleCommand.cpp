
#include "Server.hpp"

void        Server::handlePass( int fd , std::vector<std::string> args)
{
    Client *client = getClientByFd(fd);
    if(client == NULL)
        return ;
    if(args.empty() ||  args[0].empty())
    {
        sendToClient(fd, ":server 461 * PASS :Not enough parameters");
        return;
    }

    std::cout << "args[0] = [" << args[0] << "]" << std::endl;
    std::cout << "GetPassword = [" << GetPassword() <<  "]" << std::endl;

    if(args[0] == GetPassword())
        client->SetRegistration(true);
    else
    {
        sendToClient(fd, ":server 464 * :Password incorrect");
        // RemoveClinet(fd);
        return ;
    }
    std::cout << "Client " << fd << " authenticated successfully" << std::endl;
}

void    Server::handleNick( int fd , std::vector<std::string> args)
{
    Client *client = getClientByFd(fd);
    if(client == NULL)
        return ;
    if(args.empty() ||  args[0].empty())
    {
        sendToClient(fd, ":server 461 * PASS :Not enough parameters");
        return;
    }

    std::cout << "args[0] = [" << args[0] << "]" << std::endl;
    client->setNickname(args[0]);
    client->setNick(true);
}

void        Server::handleUser( int fd , std::vector<std::string> args)
{


    for (size_t i = 0; i < args.size(); i++)
    {
        std::cout  << "args[" << i << "] --> " << args[i] << std::endl;
    }
    


    Client *client = getClientByFd(fd);
    if(client == NULL)
        return ;
    if(args.empty() ||  args[0].empty() || args[3].empty())
    {
        sendToClient(fd, ":server 461 " + client->getNickname() + " USER :Not enough parameters");
        return;
    }
    client->setUsername(args[0]);
    client->setRealname(args[4]);

    sendToClient(fd , ":server 001 " + client->getNickname() + " :Welcome to the Network");
}
void        Server::handlePrivmsg( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handleJoin( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handlePart( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handleKick( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handleInvite( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handleTopic( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handleMode( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handlePing( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::cmdNotFound( int fd ,std::string cmd )
{

    Client *client = getClientByFd(fd);
    if(!client)
        return ;
    
    if(cmd == "PASS")
        return;

    std::cout << "Unknown command '" << cmd << "' from client " << fd << std::endl;
    sendToClient(fd, ":server 421 " + client->getNickname() + " " + cmd + " :Unknown command");
}

