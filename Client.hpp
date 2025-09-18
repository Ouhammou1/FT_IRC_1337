#include <iostream>

class Client
{
private:
    int fd;
    std::string nickname;
    std::string username;

public:
    Client();
    Client(int fd ,std::string nickname , std::string username);
    Client(const Client &other);
    Client& operator=(const Client &other);
    ~Client();



    int             getFd() const;
    std::string     getNickname() const;
    std::string     getUsername() const;


    void    desplay(std::string nameObj);
};


