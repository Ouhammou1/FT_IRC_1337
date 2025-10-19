#include "Server.hpp"


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
        // std::cout << "spaceCount = " << spaceCount << std::endl;
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

int serachClientsByNickname(std::vector<Client> &clients, std::string nickname)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].getNickname() == nickname)
            return clients[i].getFd();
    }
    return -1;
}



void        Server::handlePrivmsg( int fd , std::vector<std::string> args)
{
    // std::cout << BLUE << getCurrentTime() << " Client " << fd << " SSSSSSSSS: " << std::endl; 
    if(args.size() < 2 || args[0].empty() || args[1].empty())
    {
        sendToClient(fd, "461 PRIVMSG :Not enough parameters. Usage: PRIVMSG <nickname|#channel> <message>");
        return;
    }
    std::cout << BLUE << "arg[1] ="+args[1] << std::endl;
    if (args[1][0] != '#')
    {
        bool channelFound = false;
        for (size_t i = 0; i < this->channels.size(); i++)
        {
            if (this->channels[i].getnimo() == args[0])
            {
                channelFound = true;
                if (!this->channels[i].isUserInChannel(fd))
                {
                    sendToClient(fd, "442 " + args[0] + " :You're not on that channel");
                    return;
                }
                Client *sender = getClientByFd(fd);
                if (!sender)
                    return;
                std::string message;
                for (size_t j = 1; j < args.size(); j++)
                {
                    if (j > 1)
                        message += " ";
                    message += args[j];
                }
                std::deque<int> channelUsers = this->channels[i].getUsersFds();
                for (size_t k = 0; k < channelUsers.size(); k++)
                {
                    if (channelUsers[k] != fd)
                    {
                        sendToClient(channelUsers[k], ":" + sender->getNickname() + " PRIVMSG " + args[0] + " :" + message);
                    }
                }
                break;
            }
        }
        if (!channelFound)
        {
            sendToClient(fd, "403 " + args[0] + " :No such channel");
            return;
        }
    }
    else
    {
        //handle private message to user
    }
    
    Client *client = getClientByFd(fd);
    sendToClient(fd , ":" + GetName() + " 1337 " + client->getNickname() + " :privmsg cmd it recived");
    // (void)args;
    for (size_t i = 0; i < args.size(); i++)
    {
        std::cout << "args[" << i << "] = " << args[i] << std::endl;
    }
    
}