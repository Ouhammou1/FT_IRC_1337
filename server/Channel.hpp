#pragma once
#include <map>
#include <iostream>
#include <stack>
#include <set>

class Channel
{
private:
    std::string name;
    std::string topic;
    std::map<int, std::string> users;
    std::string password;
    std::set<int> operators;
    std::set<int> invited_users;
    bool topic_lock;
    bool Invite_only;
    int user_limit;
public:
    Channel(std::string nam):name(nam), topic_lock(false), Invite_only(false), user_limit(0){};
    std::string getnimo(){return this->name;};
    void setName(std::string n){ this->name = n;};
    void setPassword(std::string pass){ this->password = pass;};
    std::string getpassword(){ return this->password;};
    void addUser(int fd, std::string nickname){ this->users[fd] = nickname;};
    bool isUserInChannel(int fd){return users.find(fd) != users.end();};
    void addOperator(int fd){this->operators.insert(fd);};
    void addInvited_user(int fd){this->invited_users.insert(fd);};
    void removeUser(int fd)
    {
        users.erase(fd);
    }
    void removeOperator(int fd){this->operators.erase(fd);};
    int getLimit(){return this->user_limit;};
    void setLimit(int lim){this->user_limit = lim;};
    std::deque<int> getUsersFds()
    {
        std::deque<int> fds;
        for (std::map<int, std::string>::iterator it = users.begin(); it != users.end(); ++it)
        {
            fds.push_back(it->first);
        }
        return fds;
    }
    std::map<int, std::string> getMap(){
        return this->users;
    }
    bool checkModerator(int fd)
    {
        return operators.count(fd) > 0;
    }
    bool checkInvited_Users(int fd)
    {
        return invited_users.count(fd) > 0;
    }
    bool GetInvite_only(){return this->Invite_only;};
    void SetInvite_only(bool c){this->Invite_only = c;};
    std::string getTopic(){return this->topic;};
    void setTopic(std::string newTopic){this->topic = newTopic;};
    bool GetTopicLock(){return this->topic_lock;};
    void SetTopicLock(bool c){this->topic_lock = c;};
    std::string getModes()
    {
        std::string modes = "+";
        if (Invite_only)
            modes += "i";
        if (topic_lock)
            modes += "t";
        if (!password.empty())
            modes += "k";
        if (user_limit > 0)
            modes += "l";
    
        if (modes == "+")
            return "";
        return modes;
    }
    // void RemoveClient()
};


