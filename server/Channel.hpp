#pragma once
#include <map>
#include <iostream>
#include <stack>

class Channel
{
private:
    std::string name;
    std::map<int, std::string> users;
    std::string password;
public:
    Channel(std::string nam):name(nam){};
    std::string getnimo(){return this->name;};
    void setName(std::string n){ this->name = n;};
    void setPassword(std::string pass){ this->password = pass;};
    std::string getpassword(){ return this->password;};
    void addUser(int fd, std::string nickname){ this->users[fd] = nickname;};
    bool isUserInChannel(int fd){return users.find(fd) != users.end();};
    void removeUser(int fd)
    {
        users.erase(fd);
    }
    std::deque<int> getUsersFds()
    {
        std::deque<int> fds;
        for (std::map<int, std::string>::iterator it = users.begin(); it != users.end(); ++it)
        {
            fds.push_back(it->first);
        }
        return fds;
    }
};


