#include "Chatbot.hpp"

void Chatbot::help(int fd, const std::string & name)
{
    std::string msg = ":bot PRIVMSG " + name + " :Available commands: CALCUL(ex:CALCUL A+B-C*D/E), HELP, HOUR\r\n";
    send(fd , msg.c_str() , msg.length() , 0);
}
void Chatbot::hour(int fd, const std::string &name)
{
    time_t now = time(0);
    char* timeStr = ctime(&now);
    
    std::string timeString(timeStr);
    if (!timeString.empty() && timeString[timeString.length()-1] == '\n') {
        timeString.erase(timeString.length()-1);
    }
    std::string msg = ":bot PRIVMSG " + name + " :Current time: " + timeString + "\r\n";
    send(fd, msg.c_str(), msg.length(), 0);
}

std::string pars_op(std::string str , int *pos)
{
    std::string res;
    if ((*(pos) != 0 && (str[(*pos) -1] != '/' && str[(*pos) -1] != '*' && str[(*pos) -1] != '-' && str[(*pos) -1] != '+')) 
        && (str[(*pos)] == '*' || ( (str[(*pos)] == '+' || str[(*pos)] == '-') && *(pos) != 0) || str[(*pos)] == '/'))
    {
        res = str.substr((*pos),1);
        (*pos)++;
        return res;
    }
    if (str[(*pos)] == '+' || str[(*pos)] == '-')
    {
        res.push_back(str[(*pos)]);
        (*pos)++;
    }
    while (isdigit(str[(*pos)]))
    {
        res.push_back(str[(*pos)]);
        (*pos)++;
    }
    return res;
}
std::string ft_itoi(int nbr)
{
    std::string res;
    bool ng = false;
    if (nbr < 0)
    {
        ng = true;
        nbr *= -1;
    }
    if (nbr == 0)
    {
        res.push_back('0');
        return res;
    }
    while (nbr / 10 != 0)
    {
        res.push_back((nbr % 10) + '0');
        nbr /= 10;
    }
    res.push_back(nbr + '0');
    if (ng)
        res.push_back('-');
    std::reverse(res.begin(), res.end());
    return res;
}
std::string calcul(int n1, int n2, int op)
{
    std::string res;
    switch (op)
    {
    case '*':
            res = ft_itoi(n1 * n2);
        break;
    case '+':
        res = ft_itoi(n1 + n2);
        break;
    case '-':
        res = ft_itoi(n1 - n2);
        break;
    case '/':
    {
        if ( n2 == 0)
            break;
        res = ft_itoi(n1 / n2);
        break;
    }
    default:
        break;
    }
    return res;
}
void remove_op(std::vector<std::string> &oper, size_t i)
{
    size_t j = 0;
    while (i + 2 + j < oper.size() && i + j < oper.size())
    {
        oper[i + j] = oper[i + 2 + j];
        j++;
    }
    for (size_t k = 0; k < 2; k++)
    {
        oper.pop_back();
    }
}
void run_op(std::vector<std::string> &oper, size_t i)
{
    if (oper.size() == 3)
        return;
    std::string res;
    int n1 = std::atoi(oper[i-1].c_str());
    int n2 = std::atoi(oper[i+1].c_str());
    res = calcul(n1, n2, oper[i][0]);
    oper[i - 1] = res;
    remove_op(oper, i);
}
bool find_op(const std::vector<std::string> &oper)
{
    for (size_t i = 1; i < oper.size(); i+=2)
    {
        if (oper[i].compare("*") == 0 || oper[i].compare("/") == 0)
            return true;
    }
    return false;
}
void arith(std::vector<std::string> &oper)
{
    int i = 1;
    while (find_op(oper))
    {
        if (oper[i][0] == '*' || oper[i][0] == '/')
            run_op(oper, i);
        else    
            i += 2;
        if (oper.size() == 3)
            break;
    }
    i = 1;
    while (oper.size() > 3)
    {
        run_op(oper, i);
        i = 1;
    }
}
bool op_form(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (!isdigit(str[i]) && (str[i] != '*' && str[i] != '+' && str[i] != '-' && str[i] != '/'))
            return false;
        if (str[i] == '*' || str[i] == '/' || str[i] == '-' || str[i] == '+')
        {
            if (i + 1 < str.size() && (str[i+1] == '*' || str[i+1] == '/'))
                return false;
        }
        if (str[i] == '/' && (i + 1 < str.size() && str[i+1] == '0'))
            return false;
        if (str[str.size()-1] == '*' || str[str.size()-1] == '/' || str[str.size()-1] == '-' || str[str.size()-1] == '+')
            return false;
    }
    return true;
}
void Chatbot::calculator(int fd, std::string arg, const std::string &name)
{
    std::string str = arg.substr(7);
    std::vector<std::string> oper;
    std::string res;
    std::string msg;
    int i = 0;
    if ( arg.size() <= 6 || !op_form(str))
    {
        msg = ":bot PRIVMSG " + name + " :Bravo you overlap your IQ score \r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        return;
    }
    while ((size_t)i < str.size())
        oper.push_back(pars_op(str, &i));
    if (oper.size() < 3)
    {
        msg = ":bot PRIVMSG " + name + " :ex:CALCUL A+B-C*D/E \r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        return;
    }
    arith(oper);
    int n1 = std::atoi(oper[0].c_str());
    int n2 = std::atoi(oper[2].c_str());
    res = calcul(n1,n2,oper[1][0]);
    msg = ":bot PRIVMSG " + name + " :"+ str +" = " + res + "\r\n";
    send(fd, msg.c_str(), msg.length(), 0);
}
void Chatbot::ident_cmd(std::string arg, int fd, const std::string &client_name)
{
    std::size_t pos = arg.find("CALCUL");
    std::size_t pos2 = arg.find("calcul");
    std::string calcul;
    if (pos != std::string::npos)
    {
        calcul = arg.substr(pos, 6);
    }
    else if (pos2 != std::string::npos)
    {
        calcul = arg.substr(pos2, 6);
    }
    if (arg == "HELP" || arg == "help")
        help(fd, client_name);
    else if (arg == "HOUR" || arg == "hour")
        hour(fd, client_name);
    else if (arg.size() > 9 && (calcul == "CALCUL" || calcul == "calcul"))
        calculator(fd ,arg, client_name);
    else
    {
        std::string msg = ":bot PRIVMSG " + client_name + " :Sorry i can not help you with that\r\n";
        send(fd , msg.c_str() , msg.length() , 0);
        help(fd, client_name);
    }
}
std::string rot13(const std::string &word)
{
    std::string res;
    for (size_t i = 0; i < word.size(); i++)
    {
        if ((word[i] > 'A' && word[i] < 'M') || (word[i] > 'a' && word[i] < 'm'))
            res.push_back(word[i]+13);
        else
            res.push_back(word[i]-13);
    }
    return res;
}
bool Chatbot::supervisor(int fd, std::string args , const std::string &name)
{
    std::vector<std::string> curses;

    curses.push_back("shpx");
    curses.push_back("qvpx");
    curses.push_back("chffl");
    curses.push_back("avttn");
    curses.push_back("ovgpu");
    curses.push_back("avttre");
    for (size_t i = 0; i < curses.size(); i++)
    {
        if (args.find(rot13(curses[i])) != std::string::npos)
        {
            std::string msg = ":bot PRIVMSG " + name + " :Watch your words!!\r\n";
            send(fd , msg.c_str() , msg.length() , 0);
            return true;
        }
    }
    return false;
}

void Chatbot::sendToDest(std::vector<std::string> args, Client dest, Client sender)
{
    int destFd = dest.getFd();
    std::string targetName = args[0];
    std::string message = args[1];

    std::string msg = ":" + sender.getNickname() + "!" + sender.getUsername() + "@localhost PRIVMSG " + targetName + " :" + message + "\r\n";
    send(destFd, msg.c_str(), msg.length(), 0);
}