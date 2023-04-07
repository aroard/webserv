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

# ifndef MAX_LISTEN
#  define MAX_LISTEN 128
# endif

class Parser;
class Server;
class Client;

class Error_exception {
private:
	std::string _msg_err;
public:
	Error_exception( const std::string &msg_err ) : _msg_err(msg_err) {}
	static void bad_send( void ) { throw Error_exception("Bad send"); }
	static void socket_close( int signum ) { 
		throw Error_exception("Socket is closing");
	}
	static void interruption_server( int signum ) { 
		throw Error_exception("\rServer is down");
	}
	static void bad_post( const std::string &msg_err ) { 
		throw Error_exception(msg_err);
	}
	const char	*what( void ) const { return (_msg_err.c_str()); }
};


const std::string	g_month[] = {	"Jan", "Feb", "Mar", "Apr", \
									"May", "Jun", "Jul", "Aug", \
									"Sep", "Oct", "Nov", "Dec" };

const std::string	g_array_method[] = {"GET", "HEAD", "POST", \
										"OPTIONS", "CONNECT", "TRACE", \
										"PUT", "PATCH", "DELETE", "" };

const std::string	g_config_methode[] = {	"listen", "server_name", "root", \
											"index", "error_log", "access_log", \
											"error_page", "limit_request", \
											"method_lists", "cgi_php", "#", ""};


# include "parser.hpp"
# include "client.hpp"
# include "server.hpp"

#endif
