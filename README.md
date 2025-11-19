# 🚀 FT_IRC - Internet Relay Chat Server

<div align="center">

![IRC Server](https://img.shields.io/badge/IRC-Server-blue?style=for-the-badge)
![C++](https://img.shields.io/badge/C++-98-00599C?style=for-the-badge&logo=c%2B%2B)
![42](https://img.shields.io/badge/42-Project-000000?style=for-the-badge)

*A custom IRC server implementation in C++98 compliant with RFC standards*

</div>

---

## 📋 Table of Contents

- [About](#about)
- [Features](#features)
- [Demo](#demo)
- [Installation](#installation)
- [Usage](#usage)
- [Commands](#commands)
- [Architecture](#architecture)
- [Contributing](#contributing)

---

## 🎯 About

FT_IRC is a robust IRC (Internet Relay Chat) server implementation written in C++98. This project demonstrates advanced network programming concepts including socket programming, non-blocking I/O, and multi-client handling using poll().

---

## ✨ Features

- ✅ **Multi-client Support** - Handle multiple simultaneous connections
- ✅ **Channel Management** - Create and manage IRC channels
- ✅ **User Authentication** - Password-protected server access
- ✅ **Operator Privileges** - Channel operator commands and controls
- ✅ **Private Messaging** - Direct user-to-user communication
- ✅ **Non-blocking I/O** - Efficient connection handling with poll()
- ✅ **RFC Compliant** - Follows IRC protocol standards

---

## 🎬 Demo

### Server Startup
![Server Startup](./assets/server-startup.gif)
*Starting the IRC server with custom port and password*

### Client Connection
![Client Connection](./assets/client-connection.gif)
*Multiple clients connecting to the server*

### Channel Operations
![Channel Operations](./assets/channel-operations.gif)
*Creating channels, inviting users, and managing permissions*

### Private Messaging
![Private Messaging](./assets/private-messaging.gif)
*Sending private messages between users*

---

## 🛠️ Installation

### Prerequisites

- C++ compiler with C++98 support (g++, clang++)
- Make
- IRC client (irssi, WeeChat, HexChat, or any RFC-compliant client)

### Build

```bash
# Clone the repository
git clone https://github.com/yourusername/FT_IRC_1337.git
cd FT_IRC_1337

# Compile
make

# Clean build files
make clean

# Full clean including executable
make fclean

# Recompile
make re
```

---

## 🚀 Usage

### Starting the Server

```bash
./ircserv <port> <password>
```

**Example:**
```bash
./ircserv 6667 mySecretPassword123
```

### Connecting with IRC Client

**Using irssi:**
```bash
irssi
/connect localhost 6667 mySecretPassword123
/nick YourNickname
/join #general
```

**Using netcat (for testing):**
```bash
nc localhost 6667
PASS mySecretPassword123
NICK YourNickname
USER username 0 * :Real Name
```

---

## 📡 Commands

### User Commands

| Command | Description | Usage |
|---------|-------------|-------|
| `NICK` | Set or change nickname | `NICK <nickname>` |
| `JOIN` | Join a channel | `JOIN #<channel>` |
| `PART` | Leave a channel | `PART #<channel>` |
| `PRIVMSG` | Send message | `PRIVMSG <target> :<message>` |
| `QUIT` | Disconnect from server | `QUIT :<message>` |
| `WHO` | List users | `WHO #<channel>` |

### Operator Commands

| Command | Description | Usage |
|---------|-------------|-------|
| `KICK` | Remove user from channel | `KICK #<channel> <user>` |
| `INVITE` | Invite user to channel | `INVITE <user> #<channel>` |
| `TOPIC` | Set/view channel topic | `TOPIC #<channel> :<topic>` |
| `MODE` | Change channel modes | `MODE #<channel> <mode>` |

### Channel Modes

- `+i` - Invite-only channel
- `+t` - Topic restricted to operators
- `+k` - Channel password
- `+o` - Give operator privileges
- `+l` - User limit

---

## 🏗️ Architecture

```
FT_IRC_1337/
├── src/
│   ├── Server.cpp          # Main server logic
│   ├── Client.cpp          # Client handler
│   ├── Channel.cpp         # Channel management
│   └── Commands.cpp        # IRC command processing
├── include/
│   ├── Server.hpp
│   ├── Client.hpp
│   ├── Channel.hpp
│   └── Commands.hpp
├── assets/                 # GIFs and images
├── Makefile
└── README.md
```

---

## 🎨 Customization

This IRC server includes several customizable features:

- **Custom Welcome Message** - Personalize the server greeting
- **Channel Limits** - Configure maximum users per channel
- **Operator Commands** - Extended operator functionality
- **Color Support** - ANSI color codes in messages
- **Custom Modes** - Additional channel modes

---

## 🤝 Contributing

Contributions are welcome! Feel free to:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 📝 License

This project is part of the 42 School curriculum.

---

## 👤 Author

**Youssef**

- GitHub: [@yourusername](https://github.com/yourusername)
- 42 Intra: `your_login`

---

<div align="center">

**Made with ❤️ at 1337 Coding School**

![1337](https://img.shields.io/badge/1337-Coding_School-000000?style=for-the-badge)

</div>
