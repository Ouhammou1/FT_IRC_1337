
#include "Server.hpp"

void        Server::handlePass( int fd , std::vector<std::string> args)
{
    Client *client = getClientByFd(fd);
    if(client == NULL)
        return ;

    if(args.empty() ||  args[0].empty())
    {
        sendToClient(fd, " 461 * PASS :Not enough parameters");
        return;
    }

    // std::cout << "args[0] = [" << args[0] << "]" << std::endl;
    // std::cout << "GetPassword = [" << GetPassword() <<  "]" << std::endl;

    if(args[0] == GetPassword())
        client->SetRegistration(true);
    else
    {
        sendToClient(fd, " 464 * PASS :Password incorrect");
        // RemoveClinet(fd);
        return ;
    }
    std::cout << RED << getCurrentTime() << " Client " << fd << " authenticated successfully" << RESET <<std::endl;
}

void    Server::handleNick( int fd , std::vector<std::string> args)
{
    Client *client = getClientByFd(fd);
    if(client == NULL)
        return ;

    if(args.empty() ||  args[0].empty())
    {
        sendToClient(fd, " 461 * NICK :Not enough parameters");
        return;
    }

    client->setNickname(args[0]);
    client->setNick(true);
}

std::string    realNmae(std::vector<std::string> args)
{
    std::string realname;
    for (size_t i = 3; i < args.size(); i++)
    {
        if (!realname.empty())
            realname += " ";
        if(args[3][0] == ':')
            realname += args[3].substr(1);
        else
            realname +=args[i];
    }
    return realname;
}

void        Server::handleUser( int fd , std::vector<std::string> args)
{
    Client *client = getClientByFd(fd);
    if(client == NULL)
        return ;

    if(client->getUser() == true)
    {
        sendToClient(fd , " 462 " + client->getNickname() + "USER :You may not reregister");
        return;
    }

    if(args.empty() ||  args[0].empty() || args[3].empty())
    {
        sendToClient(fd, " 461 " + client->getNickname() + " USER :Not enough parameters");
        return;
    }
    if(!args.empty() &&   !args[3].empty() && args[3][0] != ":")
    {
        sendToClient(fd, " 461 " + client->getNickname() + " USER :Invalid Realname");
        return;
    }
    client->setUsername(args[0]);
    client->setRealname(realNmae(args));
    client->setUser(true);
    std::cout << YELLOW << getCurrentTime() << " Client " << fd << " registered with username: "  << args[0] << " and realname: " << args[3] << RESET<< std::endl;
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

    std::cout << getCurrentTime() <<  " Unknown command '" << cmd << "' from client " << fd << std::endl;
    sendToClient(fd, " 421 " + client->getNickname() + " " + cmd + " :Unknown command");
}

