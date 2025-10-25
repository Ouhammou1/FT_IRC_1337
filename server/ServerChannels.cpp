#include "Server.hpp"
#include <ctype.h>

bool Server::search_channels(std::string name)
{
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		if(this->channels[i].getnimo() == name)
		{
			// std::cout << RED << "channel name: " << this->channels[i].getnimo() << std:: endl;
			// std::cout << RED << "compare name: " << name << std:: endl;
			return true;
		}
	}
	
	return false;
}

void        Server::handleJoin( int fd , std::vector<std::string> args)
{
	std::cout << BLUE << getCurrentTime() << " Client " << fd << " issued JOIN command with args: " << std::endl;
	if (args[0].empty() || args[0].size() == 1 || args[0].find(' ') != std::string::npos ||
	args[0].find(',') != std::string::npos || args[0][0] != '#')
	{
		sendToClient(fd, "476 " + args[0] + " :Bad Channel Mask");
		return;
	}
	// std::cout << RED << "ARG[0]"+args[0] << RESET << std::endl;
	std::transform(args[0].begin(), args[0].end(), args[0].begin(), ::tolower);
	if (args.size() > 1 && args[1].find(' ') != std::string::npos)
	{
		sendToClient(fd, "464 * :Password cannot contain spaces");
		return;
	}
	Client *client = getClientByFd(fd);
	if (!client)
		return;
	if (!search_channels(args[0]))
	{
		Channel newChannel(args[0]);
		newChannel.addOperator(client->getFd());
		this->channels.push_back(newChannel);
		if (!args[1].empty())
		{
			std::cout << "Setting password for channel " << args[0] << " to " << args[1] << std::endl;
			this->channels.back().setPassword(args[1]);
		}
		std::cout << GREEN << getCurrentTime() << " Channel " << args[0] << " created." << RESET << std::endl;
	}

	for (size_t i = 0; i < this->channels.size(); i++)
	{
		if (this->channels[i].getnimo() == args[0])
		{
			if (!this->channels[i].isUserInChannel(client->getFd()))
			{
				if (this->channels[i].getLimit() > 0 && this->channels[i].getUsersFds().size() >= static_cast<size_t>(this->channels[i].getLimit()))
                {
                    sendToClient(fd, "471 " + args[0] + " :Cannot join channel (+l)");
                    std::cout << RED << getCurrentTime() << " Client " << fd << " cannot join " << args[0] << " - channel is full" << RESET << std::endl;
                    return;
                }
				if (this->channels[i].GetInvite_only())
				{
					if (this->channels[i].checkInvited_Users(client->getFd()))
					{
						if(this->channels[i].getpassword().size() == 0 ||  this->channels[i].getpassword() == args[1])
						{
							this->channels[i].addUser(client->getFd(), client->getNickname());
							std::cout << GREEN << getCurrentTime() << " Client " << fd << " joined channel " << args[0] << RESET << std::endl;
							std::deque<int> channelUsers = this->channels[i].getUsersFds();
							for (size_t k = 0; k < channelUsers.size(); k++)
							{
								sendToClient(channelUsers[k], ":" + client->getNickname() + " JOIN " + args[0]);
							}
							sendNamesList(fd, &this->channels[i]);
						}
						else{
							std::cout << RED << getCurrentTime() << " Client " << fd << " provided incorrect key for channel " << args[0] << RESET << std::endl;
							sendToClient(fd, "475 " + args[0] + " :Cannot join channel (+k) - bad key");
							return;
						}
					}
					else
					{
						sendToClient(fd, "473 " + args[0] + " :Cannot join channel (+i) - invite only");
						return;
					}
				}
				else if ((this->channels[i].getpassword() == args[1] || this->channels[i].getpassword().size() == 0))
				{
					this->channels[i].addUser(client->getFd(), client->getNickname());
					std::cout << GREEN << getCurrentTime() << " Client " << fd << " joined channel " << args[0] << RESET << std::endl;
					
					std::deque<int> channelUsers = this->channels[i].getUsersFds();
					for (size_t k = 0; k < channelUsers.size(); k++)
					{
						sendToClient(channelUsers[k], ":" + client->getNickname() + " JOIN " + args[0]);
					}
					sendNamesList(fd, &this->channels[i]);
				}
				else
				{
					std::cout << RED << getCurrentTime() << " Client " << fd << " provided incorrect key for channel " << args[0] << RESET << std::endl;
					sendToClient(fd, "475 " + args[0] + " :Cannot join channel (+k) - bad key");
					return;
				}
			}
			else
			{
				sendToClient(fd, "443 " + client->getNickname() + " " + args[0] + " :is already on channel");
				std::cout << YELLOW << getCurrentTime() << " Client " << fd << " is already in channel " << args[0] << RESET << std::endl;
			}
			break;
		}
	}
}

int serachClientsByNickname(std::vector<Client> &clients, std::string nickname)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].getNickname() == nickname)
			return clients[i].getFd();
	}
	return -1;
}



void        Server::handlePrivmsg( int fd , std::vector<std::string> args)
{
	// std::cout << BLUE << getCurrentTime() << " Client " << fd << " SSSSSSSSS: " << std::endl; 
	if(args.size() < 2 || args[0].empty() || args[1].empty())
	{
		sendToClient(fd, "461 PRIVMSG :Not enough parameters");
		return;
	}
	std::transform(args[0].begin(), args[0].end(), args[0].begin(), ::tolower);
	std::cout << BLUE << "arg[0] ="+args[0] << RESET << std::endl;
	if (args[0][0] == '#')
	{
		bool channelFound = false;
		if (this->search_channels(args[0]))
		{
			for (size_t i = 0; i < this->channels.size(); i++)
			{
				if (this->channels[i].getnimo() == args[0])
				{
					channelFound = true;
					if (!this->channels[i].isUserInChannel(fd))
					{
						sendToClient(fd, "442 " + args[0] + " :You're not on that channel");
						return;
					}
					Client *sender = getClientByFd(fd);
					if (!sender)
						return;
					std::string message;
					for (size_t j = 1; j < args.size(); j++)
					{
						if (j > 1)
							message += " ";
						message += args[j];
					}
					std::deque<int> channelUsers = this->channels[i].getUsersFds();
					for (size_t k = 0; k < channelUsers.size(); k++)
					{
						if (channelUsers[k] != fd)
						{
							sendToClient(channelUsers[k], ":" + sender->getNickname() + " PRIVMSG " + args[0] + " :" + message);
						}
					}
					break;
				}
			}
		}
		if (!channelFound)
		{
			sendToClient(fd, "403 " + args[0] + " :No such channel");
			return;
		}
	}
	else
	{
		BotClientPrivmsg(fd, args);
	}
	
	Client *client = getClientByFd(fd);
	sendToClient(fd , ":" + GetName() + " 1337 " + client->getNickname() + " :privmsg cmd it recived");
	// (void)args;
	// for (size_t i = 0; i < args.size(); i++)
	// {
	//     std::cout << "args[" << i << "] = " << args[i] << std::endl;
	// }
	
}

Channel*    Server::getChannelByName(std::string name)
{
	for (size_t i = 0; i < this->channels.size() ; i++)
	{
		if(this->channels[i].getnimo() == name)
			return  &this->channels[i];
	}
	return NULL;
}



Client*    Server::getClientByNickName(std::string nickname)
{
	for (size_t i = 0; i < this->clients.size() ; i++)
	{
		if(this->clients[i].getNickname() == nickname)
			return  &clients[i];
	}
	return NULL;
}


void Server::handleKick(int fd, std::vector<std::string> args)
{
	Client *client = getClientByFd(fd);
	if (args.size() < 2 || args[0].empty() || args[1].empty())
	{
		sendToClient(fd, "461 " + client->getNickname() + " KICK :Not enough parameters");
		return;
	}
	// std::cout << BLUE << "ARGS[1]*" + args[0] << RESET << std::endl;
	std::transform(args[0].begin(), args[0].end(), args[0].begin(), ::tolower);
	std::string newarg = args[0];
	if (args[0][0] != '#')
		newarg = '#' + args[0];
	if (this->search_channels(newarg))
	{
		Channel *chan = getChannelByName(newarg);
		if (!chan->isUserInChannel(client->getFd()))
		{
			sendToClient(fd, "442 " + args[0] + " :You're not on that channel");
			return;
		}
		if (!chan->checkModerator(client->getFd()))
		{
			sendToClient(fd, "482 " + args[0] + " :You're not channel operator");
			return;
		}
		Client *kickuser = getClientByNickName(args[1]);
		if (!kickuser)
		{
			sendToClient(fd, "401 " + args[1] + " :No such nick/channel");
			return;
		}
		if (!chan->isUserInChannel(kickuser->getFd()))
		{
			sendToClient(fd, "441 " + args[1] + " " + args[0] + " :They aren't on that channel");
			return;
		}
		chan->removeUser(kickuser->getFd());
		std::string reason = (args.size() > 2) ? args[2] : "No reason specified";
		sendToClient(kickuser->getFd(), ":" + client->getNickname() + " KICK " + args[0] + " " + args[1] + " :" + reason);
		sendToClient(fd, ":" + client->getNickname() + " KICK " + args[0] + " " + args[1] + " :" + reason);
	}
	else
	{
		sendToClient(fd, "403 " + args[0] + " :No such channel");
		return;
	}
}


void Server::handleInvite(int fd, std::vector<std::string> args)
{
	Client *client = getClientByFd(fd);
	if (args.size() < 2 || args[0].empty() || args[1].empty())
	{
		sendToClient(fd, "461 " + client->getNickname() + " INVITE :Not enough parameters");
		return;
	}
	std::transform(args[1].begin(), args[1].end(), args[1].begin(), ::tolower);
	std::string newarg = args[1];
	if (args[1][0] != '#')
		newarg = '#' + args[1];
	if (!this->search_channels(newarg))
	{
		sendToClient(fd, "403 " + newarg + " :No such channel");
		return;
	}
	Channel *chan = getChannelByName(newarg);
	if (!chan->isUserInChannel(client->getFd()))
	{
		sendToClient(fd, "442 " + newarg + " :You're not on that channel");
		return;
	}
	else if (!chan->checkModerator(client->getFd()))
	{
		sendToClient(fd, "482 " + newarg + " :You're not channel operator");
		return;
	}
	Client *invited = getClientByNickName(args[0]);
	if (!invited)
	{
		sendToClient(fd, "401 " + args[0] + " :No such nick/channel");
		return;
	}
	if (chan->isUserInChannel(invited->getFd()))
	{
		sendToClient(fd, "443 " + args[0] + " " + newarg + " :is already on channel");
		return;
	}
	chan->addInvited_user(invited->getFd());
	sendToClient(invited->getFd(), ":" + client->getNickname() + " INVITE " + args[0] + " :" + newarg);
	sendToClient(fd, "341 " + client->getNickname() + " " + args[0] + " " + newarg);
}


void Server::handleTopic(int fd, std::vector<std::string> args)
{
	Client *client = getClientByFd(fd);
	if (args.empty())
	{
		sendToClient(fd, "461 " + client->getNickname() + " TOPIC :Not enough parameters");
		return;
	}
	std::transform(args[0].begin(), args[0].end(), args[0].begin(), ::tolower);
	std::string newarg = args[0];
	if (args[0][0] != '#')
		newarg = '#' + args[0];
	Channel *channel = getChannelByName(newarg);
	if (!channel)
	{
		sendToClient(fd, "403 " + newarg + " :No such channel");
		return;
	}
	if (!channel->isUserInChannel(client->getFd()))
	{
		sendToClient(fd, "442 " + newarg + " :You're not on that channel");
		return;
	}
	if (args.size() == 1)
	{

		if (channel->getTopic().empty())
			sendToClient(fd, ":" + GetName() + " 331 " + client->getNickname() + " " + newarg + " :No topic is set");
		else
			sendToClient(fd, ":" + GetName() + " 332 " + client->getNickname() + " " + newarg + " :" + channel->getTopic());
		return;
	}
	else
	{
		if (!channel->GetTopicLock() || channel->checkModerator(client->getFd()))
		{
			channel->setTopic(args[1]);
			std::map<int, std::string> channelUsers = channel->getMap();
			for (std::map<int, std::string>::iterator it = channelUsers.begin(); it != channelUsers.end(); ++it)
			{
				sendToClient(it->first, ":" + client->getNickname() + " TOPIC " + newarg + " :" + args[1]);
			}
		}
		else
			sendToClient(fd, "482 " + newarg + " :You're not channel operator");
		return;
	}
}

void Server::sendNamesList(int fd, Channel *channel)
{
	Client *client = getClientByFd(fd);
	if (!client)
		return;
	
	std::string namesList;
	std::map<int, std::string> users = channel->getMap();
	
	for (std::map<int, std::string>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (!namesList.empty())
			namesList += " ";
		if (channel->checkModerator(it->first))
			namesList += "@";
		namesList += it->second;
	}
	sendToClient(fd, ":" + GetName() + " 353 " + client->getNickname() + " = " + channel->getnimo() + " :" + namesList);
	sendToClient(fd, ":" + GetName() + " 366 " + client->getNickname() + " " + channel->getnimo() + " :End of /NAMES list");
}



void    Server::handleMode( int fd , std::vector<std::string> args)
{
	Client *client = getClientByFd(fd);
	std::transform(args[0].begin(), args[0].end(), args[0].begin(), ::tolower);
	std::string newarg = args[0];
	if (args[0][0] != '#')
		newarg = '#' + args[0];
	
	if (!this->search_channels(newarg))
	{
		sendToClient(fd, "403 " + newarg + " :No such channel");
		return;
	}
	
	Channel *channel = getChannelByName(newarg);
	if (!channel)
		return;
	
	if (!channel->isUserInChannel(client->getFd()))
	{
		sendToClient(fd, "442 " + newarg + " :You're not on that channel");
		return;
	}
	if (args.size() == 1)
	{
		std::string modes = channel->getModes();
		sendToClient(fd, ":" + GetName() + " 324 " + client->getNickname() + " " + newarg + " " + (modes.empty() ? "+" : modes));
		return;
	}
	
	if (!channel->checkModerator(client->getFd()))
	{
		sendToClient(fd, "482 " + newarg + " :You're not channel operator");
		return;
	}
	
	std::string mode = args[1];
	size_t paramIndex = 2;
	bool adding = false;
	std::string appliedModes = "";
	std::string modeParams = "";
	
	for (size_t i = 0; i < mode.size(); i++)
	{
		char m = mode[i];
		if (m == '+')
			adding = true;
		else if (m == '-')
			adding = false;
		else if (m == 'i')
		{
			channel->SetInvite_only(adding);
			appliedModes += (adding ? "+" : "-");
			appliedModes += "i";
		}
		else if (m == 't')
		{
			channel->SetTopicLock(adding);
			appliedModes += (adding ? "+" : "-");
			appliedModes += "t";
		}
		else if (m == 'k')
		{
			if (adding)
			{
				if (paramIndex >= args.size())
				{
					sendToClient(fd, "461 " + client->getNickname() + " MODE :Not enough parameters");
					return;
				}
				if (!channel->getpassword().empty())
				{
					sendToClient(fd, "467 " + newarg + " :Channel key already set");
					return;
				}
				for (size_t j = 0; j < this->channels.size(); j++)
				{
					if (this->channels[j].getnimo() == channel->getnimo())
					{
						this->channels[j].setPassword(args[paramIndex]);
						break;
					}
				}
				appliedModes += "+k";
				modeParams += " " + args[paramIndex];
				paramIndex++;
			}
			else
			{
				if (channel->getpassword().empty())
				{
					sendToClient(fd, "467 " + newarg + " :Channel key not set");
					return;
				}
				for (size_t j = 0; j < this->channels.size(); j++)
				{
					if (this->channels[j].getnimo() == channel->getnimo())
					{
						this->channels[j].setPassword("");
						break;
					}
				}
				appliedModes += "-k";
			}
		}
		else if(m == 'o')
		{
			if (paramIndex >= args.size())
			{
				sendToClient(fd, "461 " + client->getNickname() + " MODE :Not enough parameters");
				return;
			}
			Client *targetUser = getClientByNickName(args[paramIndex]);
			if (!targetUser)
			{
				sendToClient(fd, "401 " + args[paramIndex] + " :No such nick/channel");
				paramIndex++;
				continue;
			}
			if (!channel->isUserInChannel(targetUser->getFd()))
			{
				sendToClient(fd, "441 " + args[paramIndex] + " " + newarg + " :They aren't on that channel");
				paramIndex++;
				continue;
			}
			if (adding)
			{
				if (channel->checkModerator(targetUser->getFd()))
				{
					sendToClient(fd, ":" + GetName() + " NOTICE " + client->getNickname() + " :" + args[paramIndex] + " is already an operator");
					paramIndex++;
					continue;
				}
				channel->addOperator(targetUser->getFd());
				appliedModes += "+o";
				modeParams += " " + args[paramIndex];
			}
			else
			{
				if (!channel->checkModerator(targetUser->getFd()))
				{
					sendToClient(fd, ":" + GetName() + " NOTICE " + client->getNickname() + " :" + args[paramIndex] + " is not an operator");
					paramIndex++;
					continue;
				}
				channel->removeOperator(targetUser->getFd());
				appliedModes += "-o";
				modeParams += " " + args[paramIndex];
			}
			paramIndex++;
		}
		else if(m == 'l')
		{
			if (paramIndex >= args.size())
			{
				sendToClient(fd, "461 " + client->getNickname() + " MODE :Not enough parameters");
				return;
			}
			if(adding)
			{
				channel->setLimit(atoi(args[paramIndex].c_str())); 
				appliedModes += "+l";
				modeParams += " " + args[paramIndex];
				paramIndex++;
			}
			else
			{
				channel->setLimit(0);
				appliedModes += "-l";
			}
		}
		else
		{
			sendToClient(fd, "472 " + client->getNickname() + " " + std::string(1, m) + " :is unknown mode char to me");
		}
	}
	
	if (!appliedModes.empty())
	{
		std::map<int, std::string> channelUsers = channel->getMap();
		for (std::map<int, std::string>::iterator it = channelUsers.begin(); it != channelUsers.end(); ++it)
		{
			sendToClient(it->first, ":" + client->getNickname() + " MODE " + newarg + " " + appliedModes + modeParams);
		}
	}
}
