# ifndef __MAIN_HPP__
#define __MAIN_HPP__


# include <iostream>
# include <arpa/inet.h>
# include <unistd.h>
# include <cstring> 
# include <fstream>
# include <sstream>
# include <map>
# include <vector>
# include <list>
# include <algorithm>
# include <ctime>
# include <iomanip>
# include <sys/wait.h>
# include <sys/event.h>
# include <sys/time.h>


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
class Data;
class Parser;
class Server;
class Client;


# include "error_exception.hpp"
# include "data.hpp"

# include "parser.hpp"
const std::string	Parser::_st_str_config_method[] = 
					{	"listen", "server_name", "root", \
						"index", "error_log", "access_log", \
						"error_page", "limit_request", \
						"method_lists", "cgi_php", "cgi_py", \
						"file_save", "body_limit", "#", ""};

# include "client.hpp"
const std::string	Client::_st_str_img[] = 
					{ ".jpg", ".png", ".gif", ".bmp", \
					  ".tif", ".webp", ".jpeg", ""};

const std::string	Client::_st_str_video[] = 
					{ ".mp4", ".avi", ".mov", ".wmv", \
					  ".flv", ".mkv", ".webm", ""};

# include "server.hpp"


#endif
