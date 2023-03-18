#include "Server.hpp"
#include "utils.hpp"

Server::Server(int port, std::string password) :
    port(port),
    password(password),
    maxfd(FD_SETSIZE - 1),
    is_debug(false) 
{
    struct rlimit rlp;
    xassert(getrlimit(RLIMIT_NOFILE, &rlp) != -1, "getrlimit");

    for (int i = 0; i < maxfd; i++) {
        fds.push_back(FileDescriptor(i));
    }
    populateHandleMap();
    this->create();
}

Server::~Server() {
    close(this->connection);
    close(this->sockfd);
}

void Server::create()
{
    struct sockaddr_in	sin;

    sockfd =  socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        log_fatal("Failed to create socket");
    }
    const int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        log_fatal("setsockopt(SO_REUSEADDR) failed");
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        log_fatal("setsockopt(SO_REUSEPORT) failed");
    }

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    bind(sockfd, (struct sockaddr*)&sin, sizeof(sin));
    listen(sockfd, 42);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    fds[sockfd].type = FD_SERV;
}

void	Server::init_fd()
{
    int	i = 0;
    max = 0;
    FD_ZERO(&fd_read);
    FD_ZERO(&fd_write);
    while (i < maxfd) {
        if (fds[i].type != FD_FREE) {
            FD_SET(i, &fd_read);
            User *user = getUserByFd(i);
            if (user && user->hasMessage()) {
                FD_SET(i, &fd_write);
            }
            max = MAX(max, i);
        }
        i++;
    }
}

User *Server::getUserByFd(int fd) {
    try {
        return serverData.users.at(fds[fd].userInfo.username);
    } catch (const std::out_of_range&) {
        return nullptr;
    }   
}

int	Server::do_select()
{
  struct timeval timeout = {0, 0};
  int activeFdNumber = select(max + 1, &fd_read, &fd_write, NULL, &timeout);
  if (activeFdNumber < 0 ) {
    log_fatal("An error occured during select(): %s", strerror(errno));
  }
  return activeFdNumber;
}

void	Server::check_fd(int activeFdNumber)
{
    for (int i = 0; (i < maxfd) && (activeFdNumber > 0) ; i++) {
        if (FD_ISSET(i, &fd_read)) {
            fct_read(i);
        }
        if (FD_ISSET(i, &fd_write)) {
            fct_write(i);
        }
        if (FD_ISSET(i, &fd_read) ||
        FD_ISSET(i, &fd_write)) {
            activeFdNumber--;
        }
    }
}

void Server::run() {
    while (true) {
        disconnectDeadUsers();
        pingUsers();
        init_fd();
        int activeFdNumber = do_select();
        check_fd(activeFdNumber);
    }
}

void Server::fct_read(int fd) {
	if (fds[fd].type == FD_SERV) {
		srv_accept(fd);
	} else if (fds[fd].type == FD_CLIENT) {
		client_read(fd);
	} else {
		std::cout << "Error: invalid fd type in fct_read: " << fds[fd].type << std::endl;
	}
}

void Server::srv_accept(int s)
{
    int			cs;
    struct sockaddr_in	csin;
    socklen_t		csin_len;

    csin_len = sizeof(csin);
    cs = accept(s, (struct sockaddr*)&csin, &csin_len);
    std::string ipAddress = inet_ntoa(csin.sin_addr);
    int port = ntohs(csin.sin_port);
    log_info("New client #%d from %s:%d", cs, ipAddress.c_str(), port);
    fds[cs].type = FD_CLIENT;
    fds[cs].ipAddress = ipAddress;
    fds[cs].port = port;
}

void Server::client_read(int fd)
{
    int	r;
    int	i;

    std::memset(fds[fd].buf_read, 0, BUF_SIZE);
    r = recv(fd, fds[fd].buf_read, MESSAGE_MAX_LEN, 0);
    if (r <= 0) {
      close(fd);
      fds[fd].clean();
      log_info("client #%d gone away", fd);
    }
    else {
        currUser = getUserByFd(fd);
        currFd = &fds[fd];
        outputMessage = &currUser->outputMessage;
        std::stringstream streamData(currFd->buf_read);
        log_info(
            "received from %s(fd: %d): %s",
            currUser ? currUser->username.c_str() : "NO USERNAME",
            fd,
            currFd->buf_read);

        std::string str;
        while (std::getline(streamData, str, '\n')) {
            str.erase(std::remove(str.begin(), str.end(), '\r' ), str.end());
            str.erase(std::remove(str.begin(), str.end(), '\n' ), str.end());
            parse(str, fd);
        }
    }
}

void Server::parse(std::string src, int fd) {
    const char separator = ' ';
    inputMessage = new InputMessage();
    
    std::vector<std::string> outputArray;
    std::stringstream streamData(src);
    std::string val;
    int i = 0;
    while (std::getline(streamData, val, separator)) {
        if (i == 0 && val[0] != ':') {
            inputMessage->command = val;
        }
        if (i != 0) {
            inputMessage->getParams().push_back(val);
        }
        i++;
    }

    if (handleMap.count(inputMessage->command) == 1) {

        (this->*(handleMap[inputMessage->command]))();
        if (currUser) {
            currUser->updatePing();
        }
        
    }
    delete(inputMessage);
}

void Server::fct_write(int cs)
{
	client_write(cs);
}

void Server::client_write(int fd)
{
    getUserByFd(fd)->outputMessage.sendMsg(fd);
}

void Server::pingUsers() {
    std::map<std::string, User*>::iterator it;

    for (it = serverData.users.begin(); it != serverData.users.end(); it++)
    {
        User *user = it->second;
        if (user->isNeedsPing()) {
            user->doPing();
            std::string toAdd("PING :");
            toAdd += SERVER_NAME;
            user->outputMessage.add(toAdd, RPL_NONE, user->fd);
        }
    }
}

void Server::disconnectDeadUsers() {
    std::vector<User *> deletion;
    std::vector<User *>::iterator toDelete;
    std::map<std::string, User*>::iterator it;

    for (it = serverData.users.begin(); it != serverData.users.end(); it++) {
        User *user = it->second;
        if (user->isLost()) {
            deletion.push_back(it->second);
        }
    }

    for (toDelete = deletion.begin(); toDelete != deletion.end(); toDelete++) {
        log_info("%s disconnected", (*toDelete)->username.c_str());
        doQuit(*toDelete);
    }
}