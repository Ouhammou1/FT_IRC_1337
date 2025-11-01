#include "Server.hpp"
#include "Channel.hpp"
#include "../chatbot/Chatbot.hpp"

void     Server::ParseMessage(int clientFd ,std::string  msg)
{
    if(msg.empty())
        return;

    std::istringstream mes(msg);
    std::string word;
    std::vector<std::string> parts;


    while (mes >> word && !word.empty())
    {
        if(word[0] != ':')
            parts.push_back(word);
        if(word[0] == ':')
        {
            std::string removepoints = word.substr(1);
            std::string rest;

            if (std::getline(mes , rest))
                removepoints = removepoints + rest;
            if (!removepoints.empty())
                parts.push_back(removepoints);
            break;
        }
    }
    if (parts.empty())
        return ;
    
    std::string cmd = parts[0];
    std::transform(cmd.begin() , cmd.end() , cmd.begin() , ::toupper);

    std::vector<std::string> parameters(parts.begin() + 1, parts.end());    
    HandleCommand(clientFd , cmd , parameters);

}




void Server::HandleCommand(int fd, std::string cmd, std::vector<std::string> args)
{
    std::cout << getCurrentTime() << " Handling command: " << cmd << " from client fd = " << fd << std::endl << std::endl;

    Client *client = getClientByFd(fd);
    if(client == NULL)
        return;

    if(cmd == "CAP")
    {
        sendToClient(fd, " 462 " + client->getNickname() + " :CAP CAP CAP CAP CAP CAP ===");
        return ;
    }

    if (cmd == "PASS")
    {
        if(client->getRegistration() == true)
            sendToClient(fd, " 462 " + client->getNickname() + " :You may not reregister");
        else
            handlePass(fd, args);
        return;
    }

    if (cmd == "NICK")
    {
        handleNick(fd, args);
        return;
    }

    if(cmd == "USER")
    {
        if(client->getNick() == false)
        {
            sendToClient(fd, " 451 * NICK:You must set a nickname first");
            return;
        }
        handleUser(fd, args);
        return;
    }

    if (client->getRegistration() == false ||  client->getNick() == false ||  client->getUser() == false)
    {
        sendToClient(fd, " 451 " + client->getNickname() + " :You have not registered");
        return;
    }
    std::cout << GREEN <<"antoine" << RESET << std::endl;
    if (args.size() > 1 && Chatbot::supervisor(fd, args[1], client->getNickname()))
        return;
    std::cout << YELLOW <<"antoine" << RESET << std::endl;
    if(cmd == "JOIN")
    {
        std::cout << RED <<"antoine 1900" << RESET << std::endl;
        handleJoin(fd, args);
        std::cout << GREEN <<"antoine1901" << RESET << std::endl;
    }
    else if(cmd == "PRIVMSG")
        handlePrivmsg(fd, args);
    else if (cmd == "WHOIS")
        handlewhois(fd,args);
    else if(cmd == "KICK")
        handleKick(fd, args);
    else if(cmd == "INVITE")
        handleInvite(fd, args);
    else if(cmd == "TOPIC")
        handleTopic(fd, args);
    else if(cmd == "MODE")
        handleMode(fd, args);

    else
        cmdNotFound(fd, cmd);

}
