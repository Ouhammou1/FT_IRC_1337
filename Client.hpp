#include <iostream>

class Client
{
private:
    int             fd;
    std::string     nickname;
    std::string     username;
    std::string     realname;
    bool            registration;

public:
    Client();
    Client(int fd ,std::string nickname , std::string username , std::string realname , bool registration);
    Client(const Client &other);
    Client& operator=(const Client &other);
    ~Client();



    int             getFd() const;
    std::string     getNickname() const;
    std::string     getUsername() const;
    std::string     getRealname() const;

    bool            getRegistration() ;


    void            setFd(int newFD);
    void            setNickname(std::string name);
    void            setUsername(std::string name);
    void            setRealname(std::string name);
    void            SetRegistration(bool reg) ;
  



    void    display();
};


