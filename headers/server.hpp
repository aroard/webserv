#ifndef __SERVER_HPP__
# define __SERVER_HPP__


class Server
{

private:
	std::vector<size_t>	_port;
	std::vector<size_t>	_socketServer;
	Parser				_parser;

public:

	explicit Server ( Parser parser = Parser(1, NULL) ) \
		: _parser(parser) {

		struct sockaddr_in	confServer;
		std::list<size_t>	ls = _parser.get_port(0);
		int					socketServer;

		for (std::list<size_t>::const_iterator it = ls.begin(); it != ls.end(); ++it) {

			if ((socketServer = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
				perror("ERROR socket"); exit(EXIT_FAILURE);
			}

			const int optval = 1;
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
	}


	~Server() {
		for (std::vector<size_t>::const_iterator it = _socketServer.begin(); \
			it != _socketServer.end(); ++it)
			close(*it);
	}


	void	start( void ) {
		std::cout << "Server is starting..." << std::endl;
		int									nsocket = _socketServer.size();
		int									i = 0;
		int 								kq = kqueue();
		
		if (kq < 0) {
			perror("ERROR kqueue"); exit(EXIT_FAILURE);
		}

		struct kevent 				*ev_set = new struct kevent[nsocket];
		struct kevent				*ev_get = new struct kevent[nsocket];
		std::map<int, int>			mp;

		 
		for (; i < nsocket; ++i)
			EV_SET(&ev_set[i], _socketServer[i], EVFILT_READ, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, 0);

		signal(SIGINT, Error_exception::interruption_server);
		try {
			for (int nfds;;) {
				if ((nfds = kevent(kq, ev_set, nsocket, ev_get, nsocket, NULL)) < 0) {
					perror("ERROR kevent"); exit(EXIT_FAILURE);
				}
				for (int nevset, i = 0; i < nfds; ++i) {
					
					nevset = 0;	
					if (ev_get[i].flags & EV_ERROR) {
						std::cout << "ERROR: " << ev_get[i].data << std::endl;
						exit (EXIT_FAILURE);
					}
					else if (ev_get[i].flags & EV_EOF) {
						int	n = mp[ev_get[i].ident];
						EV_SET(&ev_set[n], _socketServer[n], EVFILT_READ, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, 0);
						delete ((Client *)ev_get[i].udata);
						mp.erase(ev_get[i].ident);
					}
					else
						for (; nevset < nsocket; ++nevset) {
							if (ev_get[i].ident == _socketServer[nevset]) {
								Client	*tmp = new Client(ev_get[i].ident, _port[nevset], _parser);
								mp.insert(std::pair<int, int>(tmp->get_socket_client(), nevset));
								EV_SET(&ev_set[nevset], tmp->get_socket_client(), EVFILT_READ, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, \
									(void *)tmp);
								break ;
							}
						}
					if (ev_get[i].filter == EVFILT_READ && nevset == nsocket) {
						if (!((Client *)ev_get[i].udata)->recv_request_http()) {
							int	n = mp[ev_get[i].ident];
							EV_SET(&ev_set[n], _socketServer[n], EVFILT_READ, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, 0);
							delete ((Client *)ev_get[i].udata);
							mp.erase(ev_get[i].ident);
						}
						else
							EV_SET(&ev_set[mp.at(ev_get[i].ident)], ev_get[i].ident, \
								EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, ev_get[i].udata);
					}
					else if (ev_get[i].filter == EVFILT_WRITE && nevset == nsocket) {
						((Client *)ev_get[i].udata)->send_request_http();
						int	n = mp[ev_get[i].ident];
						EV_SET(&ev_set[n], _socketServer[n], EVFILT_READ, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, 0);
						delete ((Client *)ev_get[i].udata);
						mp.erase(ev_get[i].ident);
					}
				}
			}
		} catch ( const Error_exception &e ) {
			std::cout << e.what().first << std::endl;
		}
		signal(SIGINT, SIG_DFL);
		for (std::map<int, int>::const_iterator it = mp.begin(); it != mp.end(); ++it)
			delete ((Client *)ev_set[it->second].udata);
		delete[]	ev_set;
		delete[]	ev_get;
	}


};


#endif
