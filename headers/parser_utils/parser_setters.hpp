#ifndef __PARSER_SETTERS_HPP__
# define __PARSER_SETTERS_HPP__


std::vector<std::pair<size_t, std::list<size_t> > >					_port;
std::vector<std::pair<size_t, std::string> >						_server_name;
std::vector<std::pair<size_t, std::string> >						_root;
std::vector<std::pair<size_t, std::list<std::string> > >			_index;
std::vector<std::pair<size_t, std::pair<std::string, size_t> > >	_error_log;
std::vector<std::pair<size_t, std::pair<std::string, size_t> > >	_access_log;
std::vector<std::pair<size_t, std::string> >						_error_page;
std::vector<std::pair<size_t, size_t> >								_limit_request;
std::vector<std::pair<size_t, std::list<std::string> > >			_method_lists;
std::vector<std::pair<size_t, std::string> >						_cgi_php;
std::vector<std::pair<size_t, std::string> >						_cgi_py;
std::vector<std::pair<size_t, std::string> >						_file_save;
std::vector<std::pair<size_t, size_t> >								_body_limit;


std::pair<size_t, std::list<std::string> >	parse_template_set( const std::string &tmp ) {
	size_t	pos = _data_conf.find(";") - tmp.size();
	if (pos == std::string::npos) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string				data = _data_conf.substr(tmp.size() + 1, pos);
	std::list<std::string>	ls;

	for (size_t start;;) {
		start = data.find_first_not_of(" \t\r\n;");
		pos = data.find_first_of(" \t\r\n;", start) - start;
		if (pos == std::string::npos || start == std::string::npos)
			break ;
		ls.push_back(data.substr(start, pos));
		data = data.substr(pos + start + 1);
	}
	return (std::pair<size_t, std::list<std::string> >(_nb_conf_serv, ls));
}


void	set_listen( const std::string &tmp ) {
	std::pair<size_t, std::list<std::string> >	port = parse_template_set(tmp);
	std::list<size_t>									ls;
	for (std::list<std::string>::iterator it = port.second.begin();
		it != port.second.end(); ++it) {
		if (it->find_first_not_of("0123456789") != std::string::npos) {
			std::cerr << "Syntax error: " << __FUNCTION__
				<< ": " << __LINE__ << std::endl;
			exit(EXIT_FAILURE);
		}
		ls.push_back(atoi(it->c_str()));
	}
	_port.push_back(std::pair<size_t, std::list<size_t> >(port.first, ls));
}

bool	setting_server_name( const std::string &name ){
	const std::string host = "/etc/hosts";
	const std::string host2 = "/etc/hosts.new";
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


void	set_server_name( const std::string &tmp ) {
	const std::pair<size_t, std::list<std::string> > server_name = parse_template_set(tmp);
	if (server_name.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	if(setting_server_name(server_name.second.front()) == false)
		exit(EXIT_FAILURE);
	_server_name.push_back(std::pair<size_t, std::string>(server_name.first, server_name.second.front()));
	return ;
}


void	set_root( const std::string &tmp ) {
	const std::pair<size_t, std::list<std::string> > root = parse_template_set(tmp);
	if (root.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}

	if(!parse_files(root.second.front()))
		exit(EXIT_FAILURE);
	_root.push_back(std::pair<size_t, std::string>(root.first, root.second.front()));
	return ;
}


void	set_index( const std::string &tmp ) {
	_index.push_back(parse_template_set(tmp));
	return ;
}

	
void	set_error_log( const std::string &tmp ) {
	const std::pair<size_t, std::list<std::string> > error_log = parse_template_set(tmp);
	if (error_log.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	if (!parse_logfiles(error_log.second.front()))
		exit(EXIT_FAILURE);
	_error_log.push_back(std::pair<size_t, std::pair<std::string, size_t> >(error_log.first, \
		std::pair<std::string, size_t>(error_log.second.front(), \
		count_line_in_file(error_log.second.front()))));
	return ;
}

	
void	set_access_log( const std::string &tmp ) {
	const std::pair<size_t, std::list<std::string> > access_log = parse_template_set(tmp);
	if (access_log.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	if (!parse_logfiles(access_log.second.front()))
		exit(EXIT_FAILURE);
	_access_log.push_back(std::pair<size_t, std::pair<std::string, size_t> >(access_log.first, \
		std::pair<std::string, size_t>(access_log.second.front(), \
		count_line_in_file(access_log.second.front()))));
	return ;
}


void	set_error_page( const std::string &tmp ) {
	const std::pair<size_t, std::list<std::string> > error_page = parse_template_set(tmp);
	if (error_page.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	if(!parse_files(error_page.second.front()))
		exit(EXIT_FAILURE);
	_error_page.push_back(std::pair<size_t, std::string>(error_page.first, error_page.second.front()));
	return ;
}


void	set_limit_request( const std::string &tmp ) {
	const std::pair<size_t, std::list<std::string> > limit_request = parse_template_set(tmp);
	if (limit_request.second.size() != 1 || limit_request.second.front().find_first_not_of("0123456789") != std::string::npos) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	_limit_request.push_back(std::pair<size_t, size_t>(limit_request.first, atoi(limit_request.second.front().c_str())));
}
	

void	set_method_lists( const std::string &tmp ) {
	_method_lists.push_back(parse_template_set(tmp));
	return ;
}
	
	
void	set_cgi_php( const std::string &tmp ) {
	const std::pair<size_t, std::list<std::string> > cgi_php = parse_template_set(tmp);
	if (cgi_php.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	if (!parse_files(cgi_php.second.front()))
		exit(EXIT_FAILURE);
	_cgi_php.push_back(std::pair<size_t, std::string>(cgi_php.first, cgi_php.second.front()));
	return ;
}


void	set_cgi_py( const std::string &tmp ) {
	const std::pair<size_t, std::list<std::string> > cgi_py = parse_template_set(tmp);
	if (cgi_py.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	if (!parse_files(cgi_py.second.front()))
		exit(EXIT_FAILURE);
	_cgi_py.push_back(std::pair<size_t, std::string>(cgi_py.first, cgi_py.second.front()));
	return ;
}
	
void	set_file_save( const std::string &tmp ) {
	std::pair<size_t, std::list<std::string> > file_save = parse_template_set(tmp);
	if (file_save.second.size() != 1) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	if (file_save.second.front().substr(file_save.second.front().length() - 1) != "/")
		file_save.second.front() = file_save.second.front() + "/";

	if (access(file_save.second.front().c_str(), F_OK) != 0)
	{
		std::cerr << "Error: Path of file_save is wrong" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (access(file_save.second.front().c_str(), R_OK | X_OK) != 0)
	{
		std::cerr << "Error: Path of has the wrong right" << std::endl;
		exit(EXIT_FAILURE);
	}
	_file_save.push_back(std::pair<size_t, std::string>(file_save.first, file_save.second.front()));
	return ;
}

void	set_body_limit( const std::string &tmp ) {
	const std::pair<size_t, std::list<std::string> > body_limit = parse_template_set(tmp);
	if (body_limit.second.size() != 1 || body_limit.second.front().find_first_not_of("0123456789") != std::string::npos) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	_body_limit.push_back(std::pair<size_t, size_t>(body_limit.first, atoi(body_limit.second.front().c_str())));
}

void	set_comment_line( void ) {
	_data_conf = _data_conf.substr(_data_conf.find_first_of(";"));
	return ;
}

public:
	void	set_line_error_log( const size_t num_conf, const int nb_line ) {
		if (!_error_log.empty() && num_conf < _error_log.back().first)
			_error_log[num_conf].second.second = nb_line;
	}


	void	set_line_access_log( const size_t num_conf, const int nb_line ) {
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
			std::sprintf(buffer, "%ld", data_file.size());
			data_file = std::string("Content-Type: text/html\r\nContent-Length: ") \
			+ buffer + "\r\n\r\n" + data_file;
		}
		else {
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
