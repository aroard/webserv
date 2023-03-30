# ifndef __SERVER_HPP__
#define __SERVER_HPP__


class Server
{
	typedef	struct sockaddr_in	sockaddr_in;
private:
	sockaddr_in		_confServer;
	int				_socketServer;
	int				_socketClient;
	int				_port;
	std::string		_addr;
	bool			_end;
public:
	explicit Server( std::string addr = "127.0.0.1", \
		int port = 8000 ) : _addr(addr), _port(port), _end(false) {

		if ((_socketServer = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			perror("ERROR socket"); exit(EXIT_FAILURE);
		}
		
		if ((_confServer.sin_addr.s_addr = inet_addr(_addr.c_str())) == 0) {
			perror("ERROR inet_addr"); exit(EXIT_FAILURE);
		}
		_confServer.sin_family = AF_INET;
		_confServer.sin_port = htons(_port);

		if (bind(_socketServer, (const struct sockaddr *)&_confServer, \
				sizeof(_confServer)) < 0) {
			perror("ERROR bind"); exit (EXIT_FAILURE);
		}

		if (listen(_socketServer, 10) < 0) {
			perror("ERROR listen"); exit (EXIT_FAILURE);
		}

		std::cout << "Constructor Server called" << std::endl;
	}

	~Server() {
		
		close(_socketServer);

		std::cout << "Destructor Server called" << std::endl;
	}

	void	start( void ) {
		std::cout << "Server is starting" << std::endl;

		int	epoll_fd;

		if ((epoll_fd = epoll_create1(0)) < 0) {
			perror("ERROR epoll_create1"); exit(EXIT_FAILURE);
		}

		struct epoll_event ev;

		ev.data.fd = _socketServer;
		ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;

		if (epoll_ctl(epoll_fd, \
			EPOLL_CTL_ADD, _socketServer, &ev) < 0) {
			perror("ERROR epoll_ctl"); exit(EXIT_FAILURE);
		}

		struct epoll_event 	events[10];
		int					nfds;

		while (true) {

			if ((nfds = epoll_wait(epoll_fd, events, 10, 0)) < 0) {
				perror("ERROR epoll_wait"); exit(EXIT_FAILURE);
			}

			for (int i = 0, a = 0; i < nfds; ++i) {
				std::cout << events[i].events << std::endl;
				if (events[i].events == EPOLLIN) {
					create_client();
				}
				else if (events[i].events == EPOLLRDHUP) {
					std::cout << "Disconnected" << std::endl;
					exit(EXIT_SUCCESS);
				}
			}

		}
		return ;
	}

private:
	void	create_client( void ) {
		
		if ((_socketClient = accept(_socketServer, NULL, NULL)) < 0) {
			perror("ERRPR accept"); exit(EXIT_FAILURE);
		}

		char	msg[4096];
		
		std::memset(msg, 0, sizeof(msg));
		
		if (recv(_socketClient, msg, sizeof(msg), 0) < 0) {
			perror("ERROR recv"); exit(EXIT_FAILURE);
		}
		
		parse_request_http(std::string(msg));
		
		close (_socketClient);

		return ;
	}

	void	parse_request_http( std::string msg ) {

		return ;
	}

};

/* SANS PATH */

// GET / HTTP/1.1                                                                                                                                                                      
// Host: localhost:8080                                                                                                                                                                
// Connection: keep-alive                                                                                                                                                              
// Cache-Control: max-age=0                                                                                                                                                            
// sec-ch-ua: "Microsoft Edge";v="111", "Not(A:Brand";v="8", "Chromium";v="111"                                                                                                        
// sec-ch-ua-mobile: ?0                                                                                                                                                                
// sec-ch-ua-platform: "Windows"                                                                                                                                                       
// Upgrade-Insecure-Requests: 1                                                                                                                                                        
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36 Edg/111.0.1661.54                                       
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7                                                
// Sec-Fetch-Site: none                                                                                                                                                                
// Sec-Fetch-Mode: navigate                                                                                                                                                            
// Sec-Fetch-User: ?1                                                                                                                                                                  
// Sec-Fetch-Dest: document                                                                                                                                                            
// Accept-Encoding: gzip, deflate, br                                                                                                                                                  
// Accept-Language: fr,fr-FR;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6                                                                                                                    
                                                                                                                                                                                    
// defg2

/* AVEC UN /tools/index.html */

// GET /tools/index.html HTTP/1.1                                                                                                                                                      
// Host: localhost:8080                                                                                                                                                                
// Connection: keep-alive                                                                                                                                                              
// sec-ch-ua: "Microsoft Edge";v="111", "Not(A:Brand";v="8", "Chromium";v="111"                                                                                                        
// sec-ch-ua-mobile: ?0                                                                                                                                                                
// sec-ch-ua-platform: "Windows"                                                                                                                                                       
// Upgrade-Insecure-Requests: 1                                                                                                                                                        
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36 Edg/111.0.1661.54                                       
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7                                                
// Sec-Fetch-Site: none                                                                                                                                                                
// Sec-Fetch-Mode: navigate                                                                                                                                                            
// Sec-Fetch-User: ?1                                                                                                                                                                  
// Sec-Fetch-Dest: document                                                                                                                                                            
// Accept-Encoding: gzip, deflate, br                                                                                                                                                  
// Accept-Language: fr,fr-FR;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6                                                                                                                    
                                                                                                                                                                                    
// k 


#endif
