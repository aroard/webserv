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
# include <sys/event.h>
# include <sys/time.h>
# include <string>
# include <cerrno>

# ifndef MAX_LISTEN
#  define MAX_LISTEN 128
# endif

class Parser;
class Server;
class Client;

class Error_exception {
private:
	std::string _msg_err;
	int			_error_code;
public:
	Error_exception( const std::string &msg_err, const int error_code ) 
		: _msg_err(msg_err), _error_code(error_code) {}
	static void bad_send( void ) { throw Error_exception("Bad send", 1); }
	
	static void socket_close( int signum ) { throw Error_exception("Socket is closing", 1); }
	
	static void interruption_server( int signum ) { throw Error_exception("\rServer is down", 0);}
	
	static void error( const std::string &msg_err, const int error_code ) { 
		throw Error_exception(msg_err, error_code);
	}
	const std::pair<std::string, int>	what( void ) const { 
		return (std::pair<std::string, int>(_msg_err, _error_code)); 
	}
};


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

	void	put_line( std::string line ) {
		for (std::string::iterator it = line.begin();
			it != line.end(); ++it) {
			if (*it==13)
				std::cout << "(\\r)" << std::flush;
			else if (*it==10)
				std::cout << "(\\n)" << std::endl;
			else if (*it > 31 && *it < 127)
				std::cout << *it << std::flush;
			else
				std::cout << (int)*it << std::flush;
		}
		return ;
	}

# include "parser.hpp"
# include "client.hpp"
# include "server.hpp"

#endif
