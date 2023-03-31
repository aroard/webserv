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
		
		parse_request_http(msg);
		
		close(_socketClient);

		return ;
	}


	void	parse_request_http( std::string msg ) {
		
		std::map<std::string, std::string> request;
		std::stringstream	ss(msg);
		std::string			line;
		size_t				pos;

		if (std::getline(ss, line)) {
			pos = line.find(' ');
			request[line.substr(0, pos)] = line.substr(pos + 1, line.size() - pos - 2);
		}
		while (std::getline(ss, line)) {

			if (line.size() > 1) {
				pos = line.find(":");
				request[line.substr(0, pos)] = line.substr(pos + 2, line.size() - pos - 3);
			}
		}

		// for (std::map<std::string, std::string>::iterator it = request.begin();
		// 	it != request.end(); ++it) {
		// 	std::cout << "Key [" << it->first << "] Value [" << it->second << "]" << std::endl;
		// }

		std::string array_method[] = {	"GET", "HEAD", "POST", \
										"OPTIONS", "CONNECT", "TRACE", \
										"PUT", "PATCH", "DELETE"} ;
		int a = 0;
		do {
			if (request.count(array_method[a]))
				break;
		} while (++a < 9);

		switch (a){
			case 0:
				get_request_http(request);
				std::cout << "GET" << std::endl;
				break ;
			case 1:
				std::cout << "HEAD" << std::endl;
				break ;
			case 2:
				std::cout << "POST" << std::endl;
				break ;
			case 3:
				std::cout << "OPTIONS" << std::endl;
				break ;
			case 4:
				std::cout << "CONNECT" << std::endl;
				break ;
			case 5:
				std::cout << "TRACE" << std::endl;
				break ;
			case 6:
				std::cout << "PUT" << std::endl;
				break ;
			case 7:
				std::cout << "PATCH" << std::endl;
				break ;
			case 8:
				std::cout << "DELETE" << std::endl;
				break ;
			default:
				std::cout << "Missing path" << std::endl;
				break ;
		}
		return;
	}

	void open_files(std::ifstream & open, std::string path, int which)
	{
		std::string path2;

		if (!which)
		{
			std::cout << "which" << which << std::endl;
			open.open(path.c_str());
			if(!open)
				open_files(open, path, which++);
		}
		if (which == 1)
		{
			std::cout << "which" << which << std::endl;
			path2 = path + "/index.html";
			open.open(path2.c_str());
			if(!open)
				open_files(open, path, which++);
		}
		if (which == 2)
		{
			std::cout << "which" << which << std::endl;
			path2 = path + "/index";
			open.open(path2.c_str());
			if(!open)
				open_files(open, path, which++);
		}
	}

	void	get_request_http( std::map<std::string, std::string> &request) {

		std::string	path;

		path = request["GET"];
		path = path.substr(0, path.find(" "));

		std::ifstream		web_page;
		std::string			msg;

		msg = "HTTP/1.1 200 OK\n\n";

		// index.html index index/ index/index.html

		
		open_files(web_page, "./tools", 0);
		
		if (!web_page)
		{
			std::cout << "0 fichier trouve" << std::endl;
			exit(1);
		}

		std::string		line;

		while (std::getline(web_page, line)) {
			msg += line + '\n';
		}

		



		if (send(_socketClient, msg.c_str(), msg.size(), 0) < 0) {
			perror("ERROR send"); exit(EXIT_FAILURE);
		}

		web_page.close();

		return ;
	}
};

// HTTP/1.1 200 OK
// Content-Type: text/html
// Content-Length: 1234
// Cache-Control: max-age=3600
// Set-Cookie: nom_du_cookie=valeur_du_cookie

// <!DOCTYPE html>
// <html>
// <head>
//     <title>Titre de la page</title>
// </head>
// <body>
//     <h1>Bienvenue sur ma page web !</h1>
//     <p>Ceci est le contenu de ma page.</p>
// </body>
// </html>

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
