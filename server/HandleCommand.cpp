#include "Server.hpp"
#include "../chatbot/Chatbot.hpp"
#include <fstream> 

void        Server::handlePass( int fd , std::vector<std::string> args)
{
    Client *client = getClientByFd(fd);
    if(client == NULL)
        return ;

    if(args.empty() ||  args[0].empty() || args.size() != 1)
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

    if(args.empty() ||  args[0].empty() || args.size() != 1 )
    {
        sendToClient(fd, " 461 * NICK :Not enough parameters");
        return;
    }

    std::vector<Client> clients = getClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        if(clients[i].getNickname() == args[0])
        {
            sendToClient(fd , ":" + GetName() + " " + client->getNickname() + " USER exists ");
            return;
        }
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

    if(args.size() != 4)
    {
        sendToClient(fd, " 461 " + client->getNickname() + " USER :Not enough parameters");
        return;
    }

    std::string realname =realNmae(args);
    
    client->setUsername(args[0]);
    client->setRealname(realname);
    client->setUser(true);
    std::cout << YELLOW << getCurrentTime() << " Client " << fd << " registered with username: "  << args[0] << " and realname: " << realname << RESET<< std::endl;
    
    if(client->getUser() == true )
    {
        sendToClient(fd, ":" + GetName() + " 001 " + client->getNickname() + " :Welcome to the IRC Network " + client->getUsername()+ "!");
    }
}

bool find_client(std::string arg, std::vector<Client> clients, Client &dest)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (arg.compare(clients[i].getNickname()) == 0)
        {
            dest = clients[i];
            return true;
        }
    }
    return false;
}

void        Server::BotClientPrivmsg( int fd , std::vector<std::string> args)
{
    Client *client = getClientByFd(fd);
    Client dest;
    if (args[0].compare("bot") == 0)
        Chatbot::ident_cmd(args[1], fd, client->getNickname());
    else if (args[0].compare("NickServ") == 0)
        return;
    else if (find_client(args[0], this->clients, dest))
        Chatbot::sendToDest(args, dest, *client);
    else {
        sendToClient(fd , ":" + GetName() + " 401 " + client->getNickname() + " " + args[0] + " :No such nick/channel");
        sendToClient(fd, ":" + GetName() + " NOTICE " + client->getNickname() + " :*** User " + args[0] + " is not online");
    }
}

void        Server::cmdNotFound( int fd ,std::string cmd )
{
    Client *client = getClientByFd(fd);
    if(!client)
        return ;
    std::cout << getCurrentTime() <<  " Unknown command '" << cmd << "' from client " << fd << std::endl;
    sendToClient(fd, ":" + GetName() + " 421 " + client->getNickname() + " " + cmd + " :Unknown command");
}

void Server::handlewhois(int fd , std::vector<std::string> args)
{
    Client *client = getClientByFd(fd);
    if(!client)
        return ;
    if(args.empty() || args[0].empty())
    {
        sendToClient(fd, ":" + GetName() + " 431 " + client->getNickname() + " :No nickname given");
        return;
    }
    
    // Find the target client
    Client *target = NULL;
    std::vector<Client> clients = getClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].getNickname() == args[0])
        {
            target = &clients[i];
            break;
        }
    }
    
    if(!target)
    {
        sendToClient(fd, ":" + GetName() + " 401 " + client->getNickname() + " " + args[0] + " :No such nick/channel");
        return;
    }
    sendToClient(fd, ":" + GetName() + " 311 " + client->getNickname() + " " + 
                 target->getNickname() + " " + target->getUsername() + " " + 
                 target->getIp() + " * :" + target->getRealname());
    std::cout << target->getRealname() << std::endl;
}
