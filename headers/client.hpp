#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__


class Client
{
private:
	int				_socketClient;
	const Parser	_parser;

	Client( void );

public:
	explicit Client(int socketServer, Parser parser) : _parser(parser) {
		if ((_socketClient = accept(socketServer, NULL, NULL)) < 0) {
			perror("ERRPR accept"); exit(EXIT_FAILURE);
		}
		std::cout << "Constructor Client called" << std::endl;
	}

	~Client() {
		close (_socketClient);
		std::cout << "Destructor Client called" << std::endl;
	}

	void	run( void ) {
		routine();
	}

private:

	void	routine( void ) {

		char	*msg = new char[_parser.get_limit_request(0) + 1];

		std::memset(msg, 0, _parser.get_limit_request(0));
		
		if (recv(_socketClient, msg, _parser.get_limit_request(0), 0) < 0) {
			perror("ERROR recv"); exit(EXIT_FAILURE);
		}
		
		parse_request_http(msg);
		
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

		int a = 0;
		do {
			if (request.count(g_array_method[a]))
				break;
		} while (++a < 9);

		switch (a) {
			case 0: get_request_http(request); break ;
			case 1: std::cout << "HEAD" << std::endl; break ;
			case 2: std::cout << "POST" << std::endl; break ;
			case 3: std::cout << "OPTIONS" << std::endl; break ;
			case 4: std::cout << "CONNECT" << std::endl; break ;
			case 5: std::cout << "TRACE" << std::endl; break ;
			case 6: std::cout << "PUT" << std::endl; break ;
			case 7: std::cout << "PATCH" << std::endl; break ;
			case 8: std::cout << "DELETE" << std::endl; break ;
			default: std::cout << "Missing path" << std::endl; break ;
		}
		return;
	}


	int open_files( std::ifstream &open, std::string &path, std::string &msg )
	{
		std::ifstream		file_tmp;
		std::string			array_index[] = {"index.html", "index.htm", "test.html", "index.php", "test/index.html"};
		int					i = 0;

		open.open(path.c_str());
		if (open) {
			if (path.at(path.size() - 1) != '/') {
				file_tmp.open((path + '/').c_str());
				if (file_tmp.is_open()) {
					file_tmp.close();
					path.push_back('/');
				}
			}
			if (path.at(path.size() - 1) == '/') {
				do {
					open.close();
					open.open((path + array_index[i++]).c_str());
				} while (i != 5 && !open.is_open());
			}
		}
		if (open.is_open()) {
			msg = "HTTP/1.1 200 OK\nContent-type: text/html; charset=UTF-8\nServer:" \
					+ _parser.get_server_name(0) + "\n\n";
			i = 200;
		}
		else {
			msg = "HTTP/1.1 404 Not Found\nContent-type: text/html; charset=UTF-8\nServer:" \
					+ _parser.get_server_name(0) + "\n\n";
			open.open(_parser.get_error_page(0).c_str());
			i = 404;
		}
		return i;
	}


	std::string getDateAndTime( void )
	{
		std::stringstream ss;
		std::time_t t = std::time(NULL);
		std::tm* now = std::gmtime(&t);
		
		ss << std::setw(2) << "[" << now->tm_mday << "/";
		ss << g_month[now->tm_mon] << "/";
		ss << std::setw(2) << (now->tm_year + 1900) << ":";
    	ss << std::setw(2) << std::setfill('0') << now->tm_hour << ":";
    	ss << std::setw(2) << std::setfill('0') << now->tm_min << ":";
    	ss << std::setw(2) << std::setfill('0') << now->tm_sec << " +0000]";
		return ss.str();
	}


	void	execute_cgi_php( std::string &path_php, std::string &msg ) {
		int				fd_pipe[2];
		pid_t			pid;
		const char		*ag[3] = {_parser.get_cgi_php(0).c_str(), path_php.c_str(), NULL};

		if (pipe(fd_pipe) == -1) { perror("ERROR pipe"); exit(EXIT_FAILURE); }
		if ((pid = fork()) == -1) { perror("ERROR fork"); exit(EXIT_FAILURE); }
		if (!pid) {
			if (dup2(fd_pipe[1], STDOUT_FILENO) == -1 || close(fd_pipe[1]) == -1 \
				|| close(fd_pipe[0]) == -1) {
				perror("ERROR dup2"); exit(EXIT_FAILURE);
			}
			execve(_parser.get_cgi_php(0).c_str(), const_cast<char**>(ag), NULL);
			perror("ERROR execve"); exit(EXIT_FAILURE);
		}
		close(fd_pipe[1]);
		
		char	tmp[32];

		std::memset(tmp, 0, 31);
		msg = "HTTP/1.1 200 OK\nServer:" + _parser.get_server_name(0) + "\n\n";
		if (send(_socketClient, msg.c_str(), msg.size(), 0) < 0) {
			perror("ERROR send");
		}
		while (read(fd_pipe[0], tmp, 31) > 0) { 
			if (send(_socketClient, tmp, strlen(tmp), 0) < 0) {
				perror("ERROR send");
			} 
			std::memset(tmp, 0, 31); 
		}
		close(fd_pipe[0]);
		waitpid(pid, 0, 0);
		return ;
	}


	void	get_request_http( std::map<std::string, std::string> &request) {

		std::ifstream	web_page;
		std::string		path;
		std::string		msg;
		int				error_code;

		path = request["GET"];
		path = '.' + path.substr(0, path.find(" "));

		error_code = open_files(web_page, path, msg);

		if (error_code != 404 && path.size() > 4 \
			&& !path.compare(path.size() - 4, path.size(), ".php")) {
			execute_cgi_php(path, msg);
		}
		else {
			if (send(_socketClient, msg.c_str(), msg.size(), 0) < 0) {
					perror("ERROR send");
			}
			for (std::string line; std::getline(web_page, line);) { 
				line += '\n';
				if (send(_socketClient, line.c_str(), line.size(), 0) < 0) {
					perror("ERROR send");
				}
			}
		}
		web_page.close();

		std::cout << request["Host"]  << " - -" << getDateAndTime() << " \"GET "  << request["GET"] + "\" "
			 << error_code << " " << msg.size() << " \"-\" \"" << request["User-Agent"] + "\"" << std::endl;
		
		return ;
	}

};


#endif
