#ifndef __REQUEST_GET_HPP__
# define __REQUEST_GET_HPP__


void	open_files( std::ifstream &open, std::string &path, std::string &msg )
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
						open_files(open, path, msg);
					return ;
				}
			}
		}
	}
	if (open.is_open() == false || path.at(path.size() - 1) == '/')
		Error_exception::error(_parser.get_file_not_found(), 404);
	return ;
}


void	execute_cgi_php( std::string &path_php, std::string &msg ) {
	int				fd_pipe[2];
	pid_t			pid;
	const char		*ag[3] = {_parser.get_cgi_php(0).c_str(), path_php.c_str(), NULL};

	if (pipe(fd_pipe) == -1) { perror("ERROR pipe"); 
		Error_exception::error(_parser.get_file_internal_server_error(), 500);
	}
	if ((pid = fork()) == -1) { perror("ERROR fork"); 
		Error_exception::error(_parser.get_file_internal_server_error(), 500); 
	}
	if (!pid) {
		if (dup2(fd_pipe[1], STDOUT_FILENO) == -1 || close(fd_pipe[1]) == -1 \
			|| close(fd_pipe[0]) == -1) {
			perror("ERROR dup2"); exit(EXIT_FAILURE);
		}
		execve(_parser.get_cgi_php(0).c_str(), const_cast<char**>(ag), NULL);
		perror("ERROR execve"); exit(EXIT_FAILURE);
	}
	close(fd_pipe[1]);
		
	char	tmp[2048];
	int		status;

	std::memset(tmp, 0, 2047);
	msg = "HTTP/1.1 200 OK\r\n";	
	while (read(fd_pipe[0], tmp, 2047) > 0) { 
		msg += tmp;
		std::memset(tmp, 0, 2047); 
	}
	close(fd_pipe[0]);
	waitpid(pid, &status, 0);
	if (status != 0)
		Error_exception::error(_parser.get_file_internal_server_error(), 500);
	return ;
}


void	execute_cgi_python( std::string &path_php, std::string &msg ) {
	int				fd_pipe[2];
	pid_t			pid;
	const char		*ag[3] = {_parser.get_cgi_py(0).c_str(), path_php.c_str(), NULL};

	if (pipe(fd_pipe) == -1) { perror("ERROR pipe"); 
		Error_exception::error(_parser.get_file_internal_server_error(), 500);
	}
	if ((pid = fork()) == -1) { perror("ERROR fork"); 
		Error_exception::error(_parser.get_file_internal_server_error(), 500); 
	}
	if (!pid) {
		if (dup2(fd_pipe[1], STDOUT_FILENO) == -1 || close(fd_pipe[1]) == -1 \
			|| close(fd_pipe[0]) == -1) {
			perror("ERROR dup2"); exit(EXIT_FAILURE);
		}
		execve(_parser.get_cgi_py(0).c_str(), const_cast<char**>(ag), NULL);
		perror("ERROR execve"); exit(EXIT_FAILURE);
	}
	close(fd_pipe[1]);
		
	char	tmp[2048];
	int		status;

	std::memset(tmp, 0, 2047);
	msg = "HTTP/1.1 200 OK\r\n\r\n";	
	while (read(fd_pipe[0], tmp, 2047) > 0) { 
		msg += tmp;
		std::memset(tmp, 0, 2047); 
	}										// RAJOUTER CONTENT-LENGHT ?
	close(fd_pipe[0]);
	waitpid(pid, &status, 0);
	if (status != 0)
		Error_exception::error(_parser.get_file_internal_server_error(), 500);
	return ;
}

void	create_copy(std::string path, std::string name)
{
	std::string		msg;
	std::string		file = _parser.get_file_save(0);
	std::ifstream	ifs(("./tools/template/" + name).c_str());
	std::ofstream	copy(path.c_str(), std::ofstream::trunc);

	if (!ifs || !copy)
	{
		if(!ifs && !copy){
			copy.close();
			ifs.close();
		}
		else{
			if(!ifs)
				copy.close();
			if(!copy)
				ifs.close();
		}
		return;
	}
	if (file.find("www/") != std::string::npos)		
			file.replace(file.find("/www/"), 5, "/");
	while(std::getline(ifs, msg, '\r')){
		if (msg.find("FILE_SAVE") != std::string::npos)
			msg.replace(msg.find("FILE_SAVE"), 9, file);
		msg + '\r';
		copy << msg;
	}

	copy.close();
	ifs.close();
	return;
}

void	get_request_get( std::map<std::string, std::string> &request) {
	std::ifstream	web_page;
	std::string		path;
	std::string		msg;

	path = _parser.get_root(0);
	if (path[path.size() - 1] == '/')
		path.erase(path.size() - 1);
	path += request["GET"].substr(0, request["GET"].find_first_of(' '));
	path = urldecode(path);
	if (path == "./www/delete.php")
		create_copy(path, "delete.php");
	open_files(web_page, path, msg);
	if (path.size() > 4 \
		&& !path.compare(path.size() - 4, 4, ".php"))
		execute_cgi_php(path, msg);
	else if (path.size() > 3 \
		&& !path.compare(path.size() - 3, 3, ".py"))
		execute_cgi_python(path, msg);
	else {
		msg = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
		for (std::string line; std::getline(web_page, line);)
			msg += line + '\n'; 
	}
	web_page.close();
	ret_request_http(request, msg, 200);
}


#endif
