#include "Server.hpp"


void     Server::ParseMessage(int clientFd ,std::string  msg)
{
    if(msg.empty())
        return;

    std::cout  << "Parsing message from client " << clientFd << " :" << msg << std::endl;

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

    for (size_t i = 0; i < parameters.size() ; i++)
    {
        std::cout  << parameters[i] << std::endl;
    }
    
    HandleCommand(clientFd , cmd , parameters);

    // for (size_t i = 0; i < parts.size(); i++)
    // {
    //     // std::cout  << i << std::endl;
    //     std::cout << parts[i] << std::endl;
    // }
    
}


void    Server::HandleCommand(int fd  , std::string cmd  , std::vector<std::string> args)
{
    std::cout << "Handling command: " << cmd << " from " << fd << std::endl << std::endl ;


    if (cmd == "NICK")
        handleNick(fd , args);

    else if(cmd == "USER")
        handleUser(fd , args);

    else if(cmd == "PRIVMSG")
        handlePrivmsg(fd , args);

    else if(cmd == "JOIN")
        handleJoin(fd , args);

    else if(cmd == "PART")
        handlePart(fd , args);

    else if(cmd == "KICK")
        handleKick(fd , args);

    else if(cmd == "INVITE")
        handleInvite(fd , args);

    else if(cmd == "TOPIC")
        handleTopic(fd , args);

    else if(cmd == "MODE")
        handleMode(fd , args);

    else if(cmd == "PING")
        handlePing(fd , args); 
    
    else
        cmdNotFound(fd , args);
}