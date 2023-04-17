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
# include <sys/event.h>
# include <sys/time.h>
# include <string>
# include <cerrno>


# ifndef MAX_LISTEN
#  ifdef SOMAXCONN
#   define MAX_LISTEN SOMAXCONN
#  else
#  	define MAX_LISTEN 128
#  endif
# endif

# ifndef MIN_LIMIT_REQUEST
#  define MIN_LIMIT_REQUEST 1024
# endif

# ifndef MAX_LIMIT_REQUEST
#  define MAX_LIMIT_REQUEST 1048576
# endif

# ifndef MAX_BODY_LIMIT
#  define MAX_BODY_LIMIT 1073741824
# endif

# ifndef LEAKS
#  define LEAKS 0
# endif


class Error_exception;
class Parser;
class Server;
class Client;


const std::string	g_month[] = {	"Jan", "Feb", "Mar", "Apr", \
									"May", "Jun", "Jul", "Aug", \
									"Sep", "Oct", "Nov", "Dec" };

const std::string	g_config_methode[] = {	"listen", "server_name", "root", \
											"index", "error_log", "access_log", \
											"error_page", "limit_request", \
											"method_lists", "cgi_php", "cgi_py", \
											"file_save", "body_limit", "#", ""};

const std::string	g_img[] = {	".jpg", ".png", ".gif", \
									".bmp", ".tif", ".webp", ".jpeg", ""};

const std::string	g_video[] = { ".mp4", ".avi", ".mov", ".wmv", \
									".flv", ".mkv", ".webm", ""};

# include "error_exception.hpp"
# include "parser.hpp"
# include "client.hpp"
# include "server.hpp"


#endif
