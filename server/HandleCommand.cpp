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
    client->setUsername(args[0]);
    client->setRealname(realname);
    std::cout << " realNmae(args) " << realname << std::endl;
    client->setUser(true);
    std::cout << YELLOW << getCurrentTime() << " Client " << fd << " registered with username: "  << args[0] << " and realname: " << realname << RESET<< std::endl;
}
void        Server::handlePrivmsg( int fd , std::vector<std::string> args)
{

    Client *client = getClientByFd(fd);

     sendToClient(fd , ":" + GetName() + " 1337 " + client->getNickname() + " :privmsg cmd it recived");
    (void) fd;
    (void) args;
}

bool Server::search_channels(std::string name)
{
    for (size_t i = 0; i < this->channels.size(); i++)
    {
        if(this->channels[i].getnimo() == name)
            return true;
    }
    
    return false;
}

void        Server::handleJoin( int fd , std::vector<std::string> args)
{
    std::cout << BLUE << getCurrentTime() << " Client " << fd << " issued JOIN command with args: " << std::endl;
    if (args[0] == "#")
    {
        sendToClient(fd, "461 JOIN :Not enough parameters. Usage: JOIN <channel> [key]");
        return;
    }

    int spaceCount = 0;
    for (size_t i = 0; i < args[1].size(); i++)
    {
        if (args[1][i] == ' ')
            spaceCount++;
    }
    if (spaceCount > 0)
    {
        std::cout << "spaceCount = " << spaceCount << std::endl;
        sendToClient(fd, "451 * JOIN :Invalid format, too many parameters");
        return;
    }

    Client *client = getClientByFd(fd);
    if (!client)
        return;

    if (!search_channels(args[0]))
    {
        Channel newChannel(args[0]);
        this->channels.push_back(newChannel);
        if (!args[1].empty())
        {
            std::cout << "Setting password for channel " << args[0] << " to " << args[1] << std::endl;
            this->channels.back().setPassword(args[1]);
        }
        std::cout << GREEN << getCurrentTime() << " Channel " << args[0] << " created." << RESET << std::endl;
        
    }

    for (size_t i = 0; i < this->channels.size(); i++)
    {
        if (this->channels[i].getnimo() == args[0])
        {
            if (!this->channels[i].isUserInChannel(client->getFd()))
            {
                if (this->channels[i].getpassword() == args[1] || this->channels[i].getpassword().empty())
                {
                    this->channels[i].addUser(client->getFd(), client->getNickname());
                    std::cout << GREEN << getCurrentTime() << " Client " << fd << " joined channel " << args[0] << RESET << std::endl;
                    sendToClient(fd, ":" + client->getNickname() + " JOIN " + args[0]);
                }
                else
                {
                    std::cout << RED << getCurrentTime() << " Client " << fd << " provided incorrect key for channel " << args[0] << RESET << std::endl;
                    sendToClient(fd, "475 " + args[0] + " :Cannot join channel (incorrect key)");
                    return;
                }
            }
            else
            {
                sendToClient(fd, "443 " + client->getNickname() + " " + args[0] + " :is already on channel");
                std::cout << YELLOW << getCurrentTime() << " Client " << fd << " is already in channel " << args[0] << RESET << std::endl;
            }
            break;
        }
    }
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

