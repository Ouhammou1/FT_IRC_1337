
#include "Server.hpp"

void        Server::handleNick( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}

void        Server::handleUser( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handlePrivmsg( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handleJoin( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handlePart( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handleKick( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handleInvite( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handleTopic( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handleMode( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::handlePing( int fd , std::vector<std::string> args)
{
    (void) fd;
    (void) args;
}
void        Server::cmdNotFound( int fd , std::vector<std::string> args)
{

    std::cout << "Command Invalid  from client : " << fd << std::endl << std::endl;
    (void) fd;
    (void) args;
}

