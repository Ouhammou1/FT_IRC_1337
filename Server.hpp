#include <iostream>
#include "Client.hpp"
#include <vector>


class Server
{
private:
    int port;
    int fd;
    std::vector<Client> clients;
    static bool signal;

  

public:
    // Server(int fd);
    // Server(const Server &other);
    // Server& operator=(const Server &other);
    Server();
    ~Server();
    
    void    StartServer();
    void    SetPort(std::string& str);

    int    GetPort();
    int    GetFd();



};



