# ifndef __SERVER_HPP__
#define __SERVER_HPP__


class Server
{

private:
	std::vector<int>	_port;
	std::list<int>		_socketServer;
	int					_socketClient;
	Parser				_parser;

public:

	explicit Server ( Parser parser = Parser(1, NULL) ) \
		: _parser(parser) {

		struct sockaddr_in	confServer;
		std::list<int>		ls = _parser.get_port(0);
		int					socketServer;

		for (std::list<int>::iterator it = ls.begin(); it != ls.end(); ++it) {

			if ((socketServer = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
				perror("ERROR socket"); exit(EXIT_FAILURE);
			}

			int optval = 1;
        	setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

			confServer.sin_addr.s_addr = htonl(INADDR_ANY);
			confServer.sin_family = AF_INET;
			confServer.sin_port = htons(*it);

			_port.push_back(*it);

			if (bind(socketServer, (const struct sockaddr *)&confServer, \
					sizeof(confServer)) < 0) {
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
		std::list<int>::iterator	it = _socketServer.begin();
		std::list<int>::iterator	ite = _socketServer.end();
		int							nsocket = _socketServer.size();
		int 						kq = kqueue();
		int							i = 0;
		
		if (kq < 0) {
			perror("ERROR kqueue"); exit(EXIT_FAILURE);
		}

		struct kevent 				*ev_set = new struct kevent[nsocket];
		struct kevent				*ev_get = new struct kevent[nsocket];
		Client						**cl = new Client*[nsocket];
		std::map<int, int>			mp;

		 
		for (; it != ite; ++it, ++i)
			EV_SET(&ev_set[i], *it, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, 0);

		signal(SIGINT, Error_exception::interruption_server);
		try {
			for (int nfds;;) {
				if ((nfds = kevent(kq, ev_set, nsocket, ev_get, nsocket, NULL)) < 0) {
					perror("ERROR kevent"); exit(EXIT_FAILURE);
				}
				for (int nevset, i = 0; i < nfds; ++i) {
					nevset = 0;
					for (it = _socketServer.begin(); it != ite; ++it, ++nevset) {
						if (ev_get[i].ident == *it) {
							for (int x = 0; x < nsocket; ++x, ++it) {
								if (x == nevset) {
									cl[nevset] = new Client(ev_get[i].ident, _port[nevset], _parser);
									mp.insert(std::pair<int, int>(cl[nevset]->get_socket_client(), nevset));
									EV_SET(&ev_set[nevset], cl[nevset]->get_socket_client(), EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, 0);
								}
								else
									EV_SET(&ev_set[x], ev_set[x].ident, EVFILT_READ, EV_DISABLE, 0, 0, 0);
							}
							break ;
						}
					}
					if (ev_get[i].filter == EVFILT_READ && nevset == nsocket) {
						if (!cl[mp[ev_get[i].ident]]->recv_request_http()) {
							it = _socketServer.begin();
							for (int x = 0; x < nsocket; ++x, ++it) {
								if (x == mp[ev_get[i].ident]) {
									cl[mp[ev_get[i].ident]]->send_request_http();
									EV_SET(&ev_set[mp[ev_get[i].ident]], *it, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, 0);
								}
								else if (!mp.count(ev_set[x].ident))
									EV_SET(&ev_set[x], *it, EVFILT_READ, EV_ENABLE | EV_ONESHOT, 0, 0, 0);
							}
							delete cl[mp[ev_get[i].ident]];
							mp.erase(ev_get[i].ident);
						}
						else
							EV_SET(&ev_set[mp[ev_get[i].ident]], ev_get[i].ident, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, 0);
					}
					else if (ev_get[i].filter == EVFILT_WRITE && nevset == nsocket) {
						it = _socketServer.begin();
						for (int x = 0; x < nsocket; ++x, ++it) {
							
							if (x == mp[ev_get[i].ident]) {
								cl[mp[ev_get[i].ident]]->send_request_http();
								EV_SET(&ev_set[mp[ev_get[i].ident]], *it, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, 0);
							}
							else
								EV_SET(&ev_set[x], *it, EVFILT_READ, EV_ENABLE | EV_ONESHOT, 0, 0, 0);
						}
						delete cl[mp[ev_get[i].ident]];
						mp.erase(ev_get[i].ident);
					}
				}
			}
		} catch ( ... ) {}
		std::cout << "\rServer is down" << std::endl;
		signal(SIGINT, SIG_DFL);
		delete 		ev_set;
		delete 		ev_get;
		delete[]	cl;
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
