#include <iostream>

class Client
{
private:
    int fd;
    std::string     nickname;
    std::string     username;
    std::string     realname;
    bool            registration;

public:
    Client();
    Client(int fd ,std::string nickname , std::string username , std::string realname);
    Client(const Client &other);
    Client& operator=(const Client &other);
    ~Client();



    int             getFd() const;
    std::string     getNickname() const;
    std::string     getUsername() const;
    std::string     getRealname() const;



    void    display(std::string nameObj);
};


