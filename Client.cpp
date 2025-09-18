
#include "Client.hpp"


int     Client::getFd() const
{
    return fd;
}


std::string Client::getNickname() const
{
        return nickname;
}
std::string Client::getUsername() const
{
        return username;
}
Client::Client(int fd ,std::string nickname , std::string username):fd(fd) , nickname(nickname) , username(username)
{

}

Client::Client() : fd(-1) , nickname("") , username("")
{

}

Client::Client(const Client &other)
{
    *this  = other;
}

Client& Client::operator=(const Client &other)
{
    if(this != &other)
    {
        fd = other.fd;
        nickname = other.nickname;
        username = other.username;
    }
    return *this;
}

Client::~Client()
{
}

void    Client::desplay(std::string nameObj)
{
    std::cout << "------------  " << nameObj << "  -------------" << std::endl; 
    std::cout << "fd  = " << fd << std::endl;
    std::cout << "nickname  = " << nickname << std::endl;
    std::cout << "username  = " << username << std::endl;
    std::cout << "\n\n\n" ;


}
