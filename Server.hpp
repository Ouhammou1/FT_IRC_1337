#include <iostream>
#include "Client.hpp"
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>


class Server
{
private:
    int     fd;
    int     port;
    std::string     password;
    std::vector<Client> clients;
    static bool signal ;
    std::vector<struct pollfd> fds;

  

public:
    // Server(int fd);
    // Server(const Server &other);
    // Server& operator=(const Server &other);
    Server();
    ~Server();
    
    void    StartServer();
    void    SetPort(std::string& str);
    void    SetPassword(std::string password);

    int            GetFd();
    int   GetPort();
    std::string    GetPassword();

    static void     SignalHandel(int signal);
    void            CreateSocket();

    void     AcceptNewClient();
    void     ReceiveNewData(int clientFd);
};



