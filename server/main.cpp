#include <iostream>
#include <csignal>
#include "Server.hpp"
#include <stdexcept>


bool PortNumber(std::string &str)
{
    if(str.empty())
        return false;
    for (size_t i = 0; i < str.size(); i++)
    {
        if(!std::isdigit(static_cast<unsigned char>(str[i])))
            return false;
    }
    return true;
}



int main(int ac , char **av)
{

    if(ac != 3)
    {
        std::cout <<  "Invalid argument. Usage: " <<av[0] << " <PORT> <PASSWORD>"  << std::endl ;
        return 0;
    }
    Server server;
    try
    {

        std::string str = av[1];
        std::string password = av[2];

        if(!PortNumber(str))
            throw std::invalid_argument("Port must be a numeric value");
    
        server.setNameServer();
        signal(SIGINT , Server::SignalHandel);
        signal(SIGQUIT , Server::SignalHandel);
        server.SetPort(str);
        server.SetPassword(password);
        std::cout << BLUE<< "Server "<< server.GetName() << " listening on port "<<server.GetPort() <<"..."<< RESET << std::endl << std::endl ;
        server.StartServer();


    }
    catch(const std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}