#include "Server.hpp"


void     Server::ParseMessage(int clientFd ,std::string  msg)
{
    if(msg.empty())
        return;

    std::istringstream mes(msg);
    std::string word;
    std::vector<std::string> parts;


    while (mes>> word && !word.empty())
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

    // for (size_t i = 0; i < parameters.size() ; i++)
    // {
    //     std::cout  << parameters[i] << std::endl;
    // }
    
    HandleCommand(clientFd , cmd , parameters);

    // for (size_t i = 0; i < parts.size(); i++)
    // {
    //     // std::cout  << i << std::endl;
    //     std::cout << parts[i] << std::endl;
    // }
    
}




void Server::HandleCommand(int fd, std::string cmd, std::vector<std::string> args)
{
    std::cout << getCurrentTime() << " Handling command: " << cmd << " from client fd = " << fd << std::endl << std::endl;

    Client *client = getClientByFd(fd);
    if(client == NULL)
        return;

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
        if(client->getRegistration() == false)
        {
            sendToClient(fd, " 451 * :You have not registered");
            return;
        }
        handleNick(fd, args);
        return;
    }

    if(cmd == "USER")
    {
        if(client->getRegistration() == false)
        {
            sendToClient(fd, " 451 * :You have not registered");
            return;
        }
        if(client->getNick() == false)
        {
            sendToClient(fd, " 451 * :You must set a nickname first");
            return;
        }
        handleUser(fd, args);
        
        if(client->getUser() == true)
        {
            sendToClient(fd, " 001 " + client->getNickname() + 
                " :Welcome to the IRC Network, " + client->getNickname() + 
                "!" + client->getUsername() + "@localhost");
            sendToClient(fd, " 002 " + client->getNickname() + 
                " :Your host is " + GetName());
        }
        return;
    }

    if (client->getRegistration() == false ||  client->getNick() == false ||  client->getUser() == false)
    {
        sendToClient(fd, " 451 " + client->getNickname() + " :You have not registered");
        return;
    }

    if(cmd == "JOIN")
        handleJoin(fd, args);
    else if(cmd == "PRIVMSG")
        handlePrivmsg(fd, args);

        
    // else if(cmd == "PART")
    //     handlePart(fd, args);
    else if(cmd == "KICK")
        handleKick(fd, args);
    else if(cmd == "INVITE")
        handleInvite(fd, args);
    else if(cmd == "TOPIC")
        handleTopic(fd, args);
    else if(cmd == "MODE")
        handleMode(fd, args);
    // else if(cmd == "PING")
    //     handlePing(fd, args);
    else
        cmdNotFound(fd, cmd);

    // client->display();
}