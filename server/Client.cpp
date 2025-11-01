
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

Client::Client(int fd ,std::string nickname , std::string username ,std::string realname , bool registration ,bool nick , bool user):fd(fd) , nickname(nickname) , username(username) , realname(realname), registration(registration) , nick(nick) , user(user)
{

}

Client::Client() : fd(-1) , nickname("") , username("") , realname(""), registration(false) , nick(false), user(false)
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
        ip              = other.ip;
        nickname        = other.nickname;
        username        = other.username;
        realname        = other.realname;
        registration    = other.registration;
        nick            = other.nick;
        user            = other.user;
    }
    return *this;
}

Client::~Client()
{
}

void    Client::setBoolian()
{
    SetRegistration(false);
    setNick(false);
    setUser(false);
}

bool     Client::getRegistration() const 
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


bool            Client::getNick() const
{
    return nick;
}
bool            Client::getUser() const 
{
    return user;
}

void            Client::setNick(bool nick)
{
        this->nick = nick;
}
void            Client::setUser(bool user)
{
        this->user = user;
}

void    Client::display()
{

    std::cout << "fd  = " << fd << std::endl;
    std::cout << "nickname  = " << nickname << std::endl;
    std::cout << "username  = " << username << std::endl;
    std::cout << "realname  = " << realname << std::endl;
    std::cout << "registration  flags = " << registration << std::endl;
    std::cout << "nick flags = " << nick << std::endl;
    std::cout << "user flags  = " << user  << std::endl;


    std::cout << "\n\n\n" ;


}

void            Client::setIp(std::string newIp)
{
    ip = newIp;
}

std::string     Client::getIp() const
{
    return ip;
}

