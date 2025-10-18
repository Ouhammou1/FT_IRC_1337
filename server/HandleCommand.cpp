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

    if(args.size() < 4)
    {
        sendToClient(fd, " 461 " + client->getNickname() + " USER :Not enough parameters");
        return;
    }

    std::string realname =realNmae(args);

    std::vector<Client> clients = getClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        if(clients[i].getUsername() == args[0])
        {
            clients[i].display();
            sendToClient(fd , ":" + GetName() + " " + client->getNickname() + " USER exists ");
            return;
        }
    }
    
    client->setUsername(args[0]);
    client->setRealname(realname);
    std::cout << " realNmae(args) " << realname << std::endl;
    client->setUser(true);
    std::cout << YELLOW << getCurrentTime() << " Client " << fd << " registered with username: "  << args[0] << " and realname: " << realname << RESET<< std::endl;
    
    if(client->getUser() == true )
        sendToClient(fd, ":" + GetName() + " 001 " + client->getNickname() + " :Welcome to the IRC Network " + client->getUsername()+ "!");
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

