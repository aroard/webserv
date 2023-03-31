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
# include <ctime>
# include <iomanip>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>

class Server;
class Client;

const std::string	g_month[] = {	"Jan", "Feb", "Mar", "Apr", \
									"May", "Jun", "Jul", "Aug", \
									"Sep", "Oct", "Nov", "Dec" };

const std::string	g_array_method[] = {"GET", "HEAD", "POST", \
										"OPTIONS", "CONNECT", "TRACE", \
										"PUT", "PATCH", "DELETE" } ;

# include "client.hpp"
# include "server.hpp"

#endif
