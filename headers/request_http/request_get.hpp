#ifndef __REQUEST_POST_HPP__
# define __REQUEST_POST_HPP__

int open_files( std::ifstream &open, std::string &path, std::string &msg )
{
	std::ifstream		file_tmp;
	int					i = 0;

	if (open.is_open())
		open.close();
	open.open(path.c_str());
	if (open.is_open()) {
		if (path.at(path.size() - 1) != '/') {
			file_tmp.open((path + '/').c_str());
			if (file_tmp.is_open()) {
				file_tmp.close();
				path.push_back('/');
			}
		}
		if (path.at(path.size() - 1) == '/') {
			std::list<std::string>	ls = _parser.get_index(0);
			for (std::list<std::string>::iterator it = ls.begin(); \
				it != ls.end(); ++it) {
				open.close();
				open.open((path + *it).c_str());
				if (open.is_open()) {
					path += *it;
					if (path.at(path.size() - 1) == '/')
						return (open_files(open, path, msg));
					return (200);
				}
			}
		}
	}
	if (open.is_open() && path.at(path.size() - 1) != '/')
		return (200);
	msg = "HTTP/1.1 404 Not Found\nContent-type: text/html; charset=UTF-8\nServer:" \
			+ _parser.get_server_name(0) + "\n\n";
	open.open(_parser.get_error_page(0).c_str());
	return (404);
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
		
	char	tmp[128];

	std::memset(tmp, 0, 31);
	msg = "HTTP/1.1 200 OK\nServer:" + _parser.get_server_name(0) + \
			"\n";
	try {
		signal(SIGPIPE, Error_exception::socket_close); 
		if (send(_socketClient, msg.c_str(), msg.size(), MSG_NOSIGNAL) < 0)
			Error_exception::bad_send;
		while (read(fd_pipe[0], tmp, 127) > 0) { 
			if (send(_socketClient, tmp, strlen(tmp), MSG_NOSIGNAL) < 0)
				Error_exception::bad_send;
			std::memset(tmp, 0, 127); 
		}
		signal(SIGPIPE, SIG_DFL);
	} catch (const Error_exception &e) { /*std::cerr << "Error_exception: " << e.what() << std::endl;*/ }
	close(fd_pipe[0]);
	waitpid(pid, 0, 0);
	return ;
}


void	get_request_get( std::map<std::string, std::string> &request) {
	std::ifstream	web_page;
	std::string		path;
	std::string		msg;
	int				error_code;

	path = _parser.get_root(0);
	if (path[path.size() - 1] == '/')
		path.erase(path.size() - 1);
	path += request["GET"].substr(0, request["GET"].find(" "));

	error_code = open_files(web_page, path, msg);
	if (error_code != 404 && path.size() > 4 \
		&& !path.compare(path.size() - 4, path.size(), ".php")) {
		execute_cgi_php(path, msg);
	}
	else {
		if (error_code != 404)
			msg = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\n\n";
		try {
			signal(SIGPIPE, Error_exception::socket_close);
			if (send(_socketClient, msg.c_str(), msg.size(), 0) < 0)
				Error_exception::bad_send;
			for (std::string line; std::getline(web_page, line);) {
				line += '\n'; 
				if (send(_socketClient, line.c_str(), line.size(), 0) < 0)
					Error_exception::bad_send;
			}
			signal(SIGPIPE, SIG_DFL);
		} catch (const Error_exception &e) { /*std::cerr << "Error_exception: " << e.what() << std::endl;*/ }
	}
	web_page.close();

	if (error_code == 200) {
		std::ofstream access_log(_parser.get_access_log(0).c_str(), \
			std::ofstream::out | std::ofstream::app);
		access_log << request["Host"]  << " - -" << getDateAndTime() << " \"GET "  << request["GET"] + "\" "
			<< error_code << " " << msg.size() << " \"-\" \"" << request["User-Agent"] + "\"" << std::endl;
		access_log.close();
	}
	else {
		std::ofstream error_log(_parser.get_error_log(0).c_str(), \
		std::ofstream::out | std::ofstream::app);
		error_log << request["Host"]  << " - -" << getDateAndTime() << " \"GET "  << request["GET"] + "\" "
			<< error_code << " " << msg.size() << " \"-\" \"" << request["User-Agent"] + "\"" << std::endl;
		error_log.close();
	}
	return ;
}

#endif