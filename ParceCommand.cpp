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

            std::cout << "removepoints :]" << removepoints  << std::endl;
            if (std::getline(mes , rest))
                removepoints = removepoints + rest;
            if (!removepoints.empty())
                parts.push_back(removepoints);
            break;
        }
    }
    if (parts.empty())
        return ;
        
    
    for (size_t i = 0; i < parts.size(); i++)
    {
        // std::cout  << i << std::endl;
        std::cout << parts[i] << std::endl;
    }
    
}
