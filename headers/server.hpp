# ifndef __SERVER_HPP__
#define __SERVER_HPP__


class Server
{
	typedef	struct sockaddr_in	sockaddr_in;
private:
	sockaddr_in		_confServer;
	std::list<int>	_socketServer;
	int				_socketClient;
	std::string		_addr;
	const Parser	_parser;

public:

	explicit Server ( std::string addr = "127.0.0.1", Parser parser = Parser(1, NULL) ) \
		: _addr(addr), _parser(parser) {

		std::list<int>	ls = _parser.get_port(0);
		int				socketServer;

		for (std::list<int>::iterator it = ls.begin(); it != ls.end(); ++it) {

			if ((socketServer = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
				perror("ERROR socket"); exit(EXIT_FAILURE);
			}
			if ((_confServer.sin_addr.s_addr = inet_addr(_addr.c_str())) == 0) {
				perror("ERROR inet_addr"); exit(EXIT_FAILURE);
			}
			_confServer.sin_family = AF_INET;
			_confServer.sin_port = htons(*it);

			if (bind(socketServer, (const struct sockaddr *)&_confServer, \
					sizeof(_confServer)) < 0) {
				perror("ERROR bind"); exit (EXIT_FAILURE);
			}

			if (listen(socketServer, MAX_LISTEN) < 0) {
				perror("ERROR listen"); exit (EXIT_FAILURE);
			}
			_socketServer.push_back(socketServer);
		}
		// std::cout << "Constructor Server called" << std::endl;
	}


	~Server() {
		
		for (std::list<int>::iterator it = _socketServer.begin(); \
			it != _socketServer.end(); ++it)
			close(*it);

		// std::cout << "Destructor Server called" << std::endl;
	}


	void	start( void ) {
		routine_server();
	}

private:

	void	routine_server( void ) {
		std::cout << "Server is starting" << std::endl;
		int kq = kqueue();
		if (kq < 0) {
			perror("ERROR kqueue"); exit(EXIT_FAILURE);
		}

		struct kevent 	*ev_set = new struct kevent[_socketServer.size()];
		int				i = 0;
		for (std::list<int>::iterator it = _socketServer.begin(); \
			it != _socketServer.end(); ++it, ++i)
			EV_SET(&ev_set[i], *it, EVFILT_READ, EV_ADD, \
				0, 0, 0);
		if (kevent(kq, ev_set, _socketServer.size(), NULL, 0, NULL) < 0) {
			perror("ERROR kevent set"); exit(EXIT_FAILURE);
		}

		delete[] ev_set;

		struct kevent	*ev_get = new struct kevent[_socketServer.size()];
		int				nfds;

		signal(SIGINT, Error_exception::interruption_server);
		try {
			while (true) {
				if ((nfds = kevent(kq, NULL, 0, ev_get, _socketServer.size(), \
					NULL)) < 0) {
					perror("ERROR kevent get"); exit(EXIT_FAILURE);
				}
				for (i = 0; i < nfds; ++i) {
					if (ev_get[i].filter == EVFILT_READ) {
						Client	cl(ev_get[i].ident, _parser);
						cl.run();
					}
				}
			}
		} catch (const Error_exception &e) {
			// std::cout << e.what() << std::endl;
		}
		signal(SIGINT, SIG_DFL);
		delete[] ev_get;
	}

};

// 172.20.0.1 - - [31/Mar/2023:15:22:58 +0000] "GET /index.html/d HTTP/2.0" \
// 404 132 "-" "Mozilla/5.0 (X11; Linux x86_64; rv:102.0) Gecko/20100101 Firefox/102.0"

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
