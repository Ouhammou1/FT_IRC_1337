#pragma once
#include <iostream>
class Client
{
private:
    int             fd;
    std::string     ip;
    std::string     nickname;
    std::string     username;
    std::string     realname;
    bool            registration;
    bool            nick;
    bool            user;

public:
    Client();
    Client(int fd ,std::string nickname , std::string username , std::string realname , bool registration , bool nick , bool user);
    Client(const Client &other);
    Client& operator=(const Client &other);
    ~Client();



    int             getFd() const;
    std::string     getNickname() const;
    std::string     getUsername() const;
    std::string     getRealname() const;

    bool            getRegistration() const;
    bool            getNick() const ;
    bool            getUser() const;
    std::string     getIp() const;


    void            setFd(int newFD);
    void            setNickname(std::string name);
    void            setUsername(std::string name);
    void            setRealname(std::string name);

    void            SetRegistration(bool reg) ;
    void            setNick(bool nick);
    void            setUser(bool user);
    void            setIp(std::string newIp);



    void    display();
};


