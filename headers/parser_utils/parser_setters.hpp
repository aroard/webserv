#ifndef __PARSER_SETTERS_HPP__
# define __PARSER_SETTERS_HPP__


std::vector<std::pair<int, std::list<int> > >				_port;
std::vector<std::pair<int, std::string> >					_server_name;
std::vector<std::pair<int, std::string> >					_root;
std::vector<std::pair<int, std::list<std::string> > >		_index;
std::vector<std::pair<int, std::pair<std::string, int> > >	_error_log;
std::vector<std::pair<int, std::pair<std::string, int> > >	_access_log;
std::vector<std::pair<int, std::string> >					_error_page;
std::vector<std::pair<int, int> >							_limit_request;
std::vector<std::pair<int, std::list<std::string> > >		_method_lists;
std::vector<std::pair<int, std::string> >					_cgi_php;


std::pair<int, std::list<std::string> >	parse_template_set( std::string &tmp ) {
	int	pos = _data_conf.find(";") - tmp.size();
	if (pos == -1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	std::stringstream			ss(_data_conf.substr(tmp.size() + 1, pos - 1));
	std::list<std::string>		ls;

	for (std::string line; std::getline(ss, line, ' '); ls.push_back(line));
	return (std::pair<int, std::list<std::string> >(_nb_conf_serv, ls));
}


void	set_listen( std::string &tmp ) {
	std::pair<int, std::list<std::string> >	port = parse_template_set(tmp);
	std::list<int>							ls;
	for (std::list<std::string>::iterator it = port.second.begin();
		it != port.second.end(); ++it) {
		if (it->find_first_not_of("0123456789") != -1) {
			std::cerr << "Syntax error: " << __FUNCTION__
				<< ": " << __LINE__ << std::endl;
			exit(EXIT_FAILURE);
		}
		ls.push_back(atoi(it->c_str()));
	}
	_port.push_back(std::pair<int, std::list<int> >(port.first, ls));
}

bool	setting_server_name(std::string name){
	std::string host = "/etc/hosts";
	std::string host2 = "/etc/hosts.new";
	std::string line;
	std::ifstream ifs(host.c_str());
	std::ofstream ofs(host2.c_str(), std::ofstream::trunc);
	int i = 0;

	if(!ofs || !ifs || name.size() < 1) {
		if (!ofs)
			std::cout << "Error: ofs could not create" << std::endl;
		if (!ifs)
			std::cout << "Error: ifs could not open" << std::endl;
		if (name.size() < 1)
			std::cout << "Error: server name must be atleast 1 character" << std::endl;
		return (false);
	}
		
	while(std::getline(ifs, line)) {
		if (!i) {
			if (line.find(" " + name + " ") == std::string::npos)
					ofs << line + " " + name + " " << std::endl;
			else {
				if (std::remove(host2.c_str()) != 0) {
					std::cout << "could not remove the folder :" + host2 << std::endl;
					return (false);
				}				
				ifs.close();
				ofs.close();
				return (true);
			}
		}
		else
			ofs << line << std::endl;
		i++;
	}

	ifs.close();
	ofs.close();

	if (std::remove(host.c_str()) != 0) {
		std::cout << "could not remove the folder :" + host << std::endl;
		return (false);
	}
	if (std::rename(host2.c_str(), host.c_str()) != 0) {
		std::cout << "could not rename the folder :" + host2 << std::endl;
		return (false);
	}
	return (true);
}


void	set_server_name( std::string &tmp ) {
	std::pair<int, std::list<std::string> > server_name = parse_template_set(tmp);
	if (server_name.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	if(setting_server_name(server_name.second.front()) == false)
		exit(EXIT_FAILURE);
	_server_name.push_back(std::pair<int, std::string>(server_name.first, server_name.second.front()));
	return ;
}


void	set_root( std::string &tmp ) {
	std::pair<int, std::list<std::string> > root = parse_template_set(tmp);
	if (root.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}

	if(!parse_files(root.second.front()))
		exit(EXIT_FAILURE);
	_root.push_back(std::pair<int, std::string>(root.first, root.second.front()));
	return ;
}


void	set_index( std::string &tmp ) {
	_index.push_back(parse_template_set(tmp));
	return ;
}

	
void	set_error_log( std::string &tmp ) {
	std::pair<int, std::list<std::string> > error_log = parse_template_set(tmp);
	if (error_log.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	if (!parse_logfiles(error_log.second.front()))
		exit(EXIT_FAILURE);
	_error_log.push_back(std::pair<int, std::pair<std::string, int> >(error_log.first, \
		std::pair<std::string, int>(error_log.second.front(), \
		count_line_in_file(error_log.second.front()))));
	return ;
}

	
void	set_access_log( std::string &tmp ) {
	std::pair<int, std::list<std::string> > access_log = parse_template_set(tmp);
	if (access_log.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	if (!parse_logfiles(access_log.second.front()))
		exit(EXIT_FAILURE);
	_access_log.push_back(std::pair<int, std::pair<std::string, int> >(access_log.first, \
		std::pair<std::string, int>(access_log.second.front(), \
		count_line_in_file(access_log.second.front()))));
	return ;
}


void	set_error_page( std::string &tmp ) {
	std::pair<int, std::list<std::string> > error_page = parse_template_set(tmp);
	if (error_page.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	if (error_page.second.front() != "./www/error_pages/NotFound.html"){
		std::cout << "Wrong path for error_page" << std::endl; exit(EXIT_FAILURE);}
	if(!parse_files(error_page.second.front()))
		exit(EXIT_FAILURE);
	_error_page.push_back(std::pair<int, std::string>(error_page.first, error_page.second.front()));
	return ;
}


void	set_limit_request( std::string &tmp ) {
	std::pair<int, std::list<std::string> > limit_request = parse_template_set(tmp);
	if (limit_request.second.size() != 1 || limit_request.second.front().find_first_not_of("0123456789") != -1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	_limit_request.push_back(std::pair<int, int>(limit_request.first, atoi(limit_request.second.front().c_str())));
}
	

void	set_method_lists( std::string &tmp ) {
	_method_lists.push_back(parse_template_set(tmp));
	return ;
}
	
	
void	set_cgi_php( std::string &tmp ) {
	std::pair<int, std::list<std::string> > cgi_php = parse_template_set(tmp);
	if (cgi_php.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	if (cgi_php.second.front() != "./cgi_bin/php-cgi"){
		std::cout << "Wrong path for cgi_php" << std::endl; exit(EXIT_FAILURE);}
	if (!parse_files(cgi_php.second.front()))
		exit(EXIT_FAILURE);
	_cgi_php.push_back(std::pair<int, std::string>(cgi_php.first, cgi_php.second.front()));
	return ;
}

	
void	set_comment_line( std::string &tmp ) {
	_data_conf = _data_conf.substr(_data_conf.find_first_of(";"));
	return ;
}

public:
	void	set_line_error_log( size_t num_conf, int nb_line ) {
		if (!_error_log.empty() && num_conf < _error_log.back().first)
			_error_log[num_conf].second.second = nb_line;
	}


	void	set_line_access_log( size_t num_conf, int nb_line ) {
		if (!_access_log.empty() && num_conf < _access_log.back().first) {
			_access_log[num_conf].second.second = nb_line;
		}
	}

private:


std::string	_file_created; // 201
std::string	_file_bad_request; // 400
std::string	_file_unauthorized; // 401
std::string	_file_forbidden; // 403
std::string	_file_not_found; // 404
std::string	_file_methode_not_allowed; // 405
std::string	_file_internal_server_error; // 500


void	set_file_http( void ) {
	const char	*path_file[] = { "./tools/created.html", "./tools/bad_request.html", \
								 "./tools/unauthorized.html", "./tools/forbidden.html", \
								 "./tools/not_found.html", "./tools/method_not_allowed.html", \
								 "./tools/internal_server_error.html", "" };
	char		buffer[256];
	std::string	data_file;
	for (int i = 0; strlen(path_file[i]); ++i) {
		std::ifstream	file(path_file[i]);
		data_file.clear();
		if (file.is_open() != false) {
			std::memset(buffer, 0, 255);
			while (file.getline(buffer, 255)) {
				data_file += buffer;
				data_file += "\r\n";
				std::memset(buffer, 0, 255);
			}
			std::sprintf(buffer, "%d", data_file.size());
			data_file = std::string("Content-Type: text/html\r\nContent-Length: ") \
			+ buffer + "\r\n\r\n" + data_file;
		}
		else {
			// std::cout << "[[[" << path_file[i] << "]]]" << std::endl;
			data_file += "Content-Type: text/html\r\nContent-Length: 57\r\n\r\n\
<html><body>Internal file loading error</body></html>\r\n";
		}
		switch (i) {
			case 0: _file_created = "HTTP/1.1 201 Created\r\n" + data_file; break;
			case 1: _file_bad_request = "HTTP/1.1 400 Bad Request\r\n" + data_file; break;
			case 2: _file_unauthorized = "HTTP/1.1 401 Unauthorized\r\n" + data_file; break;
			case 3: _file_forbidden = "HTTP/1.1 403 Forbidden\r\n" + data_file; break;
			case 4: _file_not_found = "HTTP/1.1 404 Not Found\r\n" + data_file; break;
			case 5: _file_methode_not_allowed = "HTTP/1.1 405 Method Not Allowed\r\n" + data_file; break;
			case 6: _file_internal_server_error = "HTTP/1.1 500 Internal Server Error\r\n" + data_file; break;
			default: break ;
		};
	}
	return ;
}


#endif
