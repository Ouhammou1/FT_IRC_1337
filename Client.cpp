
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
std::string     Client::getRealname() const
{
    return realname;
}

Client::Client(int fd ,std::string nickname , std::string username ,std::string realname , bool registration):fd(fd) , nickname(nickname) , username(username) , realname(realname), registration(registration)
{

}

Client::Client() : fd(-1) , nickname("") , username("") , realname(""), registration(false)
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
        fd              = other.fd;
        nickname        = other.nickname;
        username        = other.username;
        realname        = other.realname;
        registration    = other.registration;
    }
    return *this;
}

Client::~Client()
{
}


bool     Client::getRegistration()
{
    return registration;
}
void     Client::SetRegistration(bool reg)
{
    registration = reg;
}

void            Client::setFd(int newFD)
{
    fd = newFD;
}

void     Client::setNickname(std::string name)
{
    nickname = name;
}
void     Client::setUsername(std::string name)
{
    username = name;
}
void     Client::setRealname(std::string name)
{
    realname = name;
}



void    Client::display()
{

    std::cout << "fd  = " << fd << std::endl;
    std::cout << "nickname  = " << nickname << std::endl;
    std::cout << "username  = " << username << std::endl;
    std::cout << "realname  = " << realname << std::endl;
    std::cout << "registration  = " << registration << std::endl;

    std::cout << "\n\n\n" ;


}
