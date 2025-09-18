#include <iostream>
#include "Client.hpp"
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
        std::cout <<  "Invalid argument. Usage: " <<av[0] << " <PORT> <PASSWORD>"  << std::endl;
        return 0;
    }
    try
    {
        std::string str = av[1];
        if(!PortNumber(str))
            throw std::invalid_argument("Port must be a numeric value");
    }
    catch(const std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    

}