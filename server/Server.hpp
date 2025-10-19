#include <iostream>
#include "Client.hpp"
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sstream>
#include <ctime>
#include <arpa/inet.h>
#include "Channel.hpp"

#define GREEN "\033[32m"
#define RESET "\033[0m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define BLUE  "\033[34m"


class Server
{
private:
    int                 fd;
    int                 port;
    std::string         password;
    std::string         name;
    std::vector<Client> clients;
    std::vector<Channel> channels;
    static bool         signal ;
    
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
    int            GetPort();
    std::string    GetPassword();
    std::vector<Client> getClients();

    // bool    setNameServer(char c);
    void     setNameServer();

    void            SetName(std::string name);
    std::string     GetName();

    static void     SignalHandel(int signal);
    void            CreateSocket();

    void     AcceptNewClient();
    void     ReceiveNewData(int clientFd);
    void     CloseServer();
    void     ParseMessage(int clientFd ,std::string  line);
    void     HandleCommand(int fd  , std::string cmd  , std::vector<std::string> args);


    void    handlePass( int fd , std::vector<std::string> args);
    void    handleNick( int fd , std::vector<std::string> args);
    void    handleUser( int fd , std::vector<std::string> args);
    void    handlePrivmsg( int fd , std::vector<std::string> args);
    void    BotClientPrivmsg( int fd , std::vector<std::string> args);
  
    void    cmdNotFound( int fd , std::string cmd );



    Client*    getClientByFd(int fd);



    void        sendToClient(int fd, const std::string& message);
    void        RemoveClinet(int fd);

    std::string     getCurrentTime();

    //Youssef part channels
    bool search_channels(std::string name);
    void    handleJoin( int fd , std::vector<std::string> args);
    // void    handlePart( int fd , std::vector<std::string> args);
    // void    handleKick( int fd , std::vector<std::string> args);
    // void    handleInvite( int fd , std::vector<std::string> args);
    // void    handleTopic( int fd , std::vector<std::string> args);
    // void    handleMode( int fd , std::vector<std::string> args);
    // void    handlePing( int fd , std::vector<std::string> args);
};



