#include "Server.hpp"
// #include "Client.hpp"

void    Server::CreateSocket()
{

    struct sockaddr_in address;
    struct pollfd NewPoll;

    address.sin_family =AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port=htons(GetPort());

    // printf("address.sin_port = %d \n" , address.sin_port);

    fd = socket(AF_INET , SOCK_STREAM , 0);
    if(fd == -1)
        throw std::runtime_error("Failed to create socket");
    
    int option = 1;
    if(setsockopt(fd , SOL_SOCKET , SO_REUSEADDR , &option , sizeof(option)) < 0)
    {
        close(fd);
        throw std::runtime_error("Failed to set SO_REUSEADDR ");
    }
    if(bind(fd ,  (struct sockaddr*)&address , sizeof(address)) < 0)
    {
        close(fd);
        throw std::runtime_error("Failed to bind socket ");
    }
    if (listen(fd , SOMAXCONN) < 0)
    {
        close(fd);
        throw std::runtime_error("Failed to listen");
    }
    if(fcntl(fd , F_SETFL , O_NONBLOCK) < 0)
    {
        close(fd);
        throw std::runtime_error("failed to set non-blocking");
    }

    NewPoll.fd= fd;
    NewPoll.events = POLLIN;
    NewPoll.revents = 0;
    fds.push_back(NewPoll);
}


void    Server::AcceptNewClient()
{
    Client          newClient;
    struct          sockaddr_in clientAddr;
    socklen_t       clientlen = sizeof(clientAddr);
    pollfd          mypollfd;


    int clientFD = accept(fd , (struct sockaddr*)&clientAddr , &clientlen);
    if(clientFD == -1)
    {
        perror("accept() failed");
        return ;
    }
    if (fcntl(clientFD  , F_SETFL , O_NONBLOCK ) < 0)
    {
        perror("fcntl() failed");
        close(clientFD);
        return;
    }
    
    std::cout << "New client connected: fd = " << clientFD << std::endl;


    newClient.setFd(clientFD);
    clients.push_back(newClient);

    mypollfd.fd = clientFD;
    mypollfd.events = POLLIN;
    mypollfd.revents = 0;

    fds.push_back(mypollfd);
}

void    Server::ReceiveNewData(int clientFd)
{
    char buffer[1024];
    memset(buffer , 0 , sizeof(buffer));

    ssize_t bytRead = recv(clientFd , buffer , sizeof(buffer) -1 ,  0) ;
    if (bytRead <= 0)
    {
        std::cout << "bytRead =   -------------------------------> " << bytRead << std::endl;
        if(bytRead == 0)
            std::cout << "Client " << clientFd << " disconnected." << std::endl;
        else
            perror("recv() failed ");
        
        for (size_t i = 0; i < fds.size(); i++)
        {
            if(fds[i].fd == clientFd)
            {
                fds.erase(fds.begin() + i);
                break;
            }
        }
        for (size_t i = 0; i < clients.size(); i++)
        {
            if(clients[i].getFd() == clientFd)
            {
                clients.erase(clients.begin() + i);
                break;
            }
        }
        close(clientFd);
    }
    else
    {
        buffer[bytRead] ='\0';
        std::cout << "Receied from " << clientFd  << ": " << buffer << std::endl; 


        std::string data(buffer);
        std::istringstream stream(data);
        std::string line ;

        while (std::getline(stream , line))
        {
            if(!line.empty() && line.back() == '\r')
                line.pop_back();
            // std::cout  << "Line :" << line << std::endl;
            
            if(!line.empty())
                ParseMessage(clientFd , line);
        }
        
        // send(clientFd , buffer , bytRead, 0);
    }
}

void    Server::StartServer()
{
    Server::CreateSocket();


    while (Server::signal == false)
    {
        int ret = poll(&fds[0] , fds.size() , -1 );
        if (ret == -1 && Server::signal == false)
            throw std::runtime_error("Poll() Failed ");

        for (size_t i = 0; i < fds.size() ; i++)
        {
            if(fds[i].revents & POLLIN)
            {
                if(fds[i].fd == fd)
                    AcceptNewClient();
                else
                    ReceiveNewData(fds[i].fd);
            }
        }        
    }
    
} 
