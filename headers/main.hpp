# ifndef __MAIN_HPP__
#define __MAIN_HPP__

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <cstring> 
# include <pthread.h>
# include <stdio.h>
# include <cstdlib>
# include <sys/epoll.h>
# include <fstream>
# include <sstream>
# include <map>
# include <vector>
# include <list>
# include <algorithm>

class Server;
class Client;

# include "client.hpp"
# include "server.hpp"

#endif
