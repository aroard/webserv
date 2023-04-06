#ifndef __PARSER_SETTERS_HPP__
# define __PARSER_SETTERS_HPP__

std::vector<std::pair<int, std::list<int> > >			_port;
std::vector<std::pair<int, std::string> >				_server_name;
std::vector<std::pair<int, std::string> >				_root;
std::vector<std::pair<int, std::list<std::string> > >	_index;
std::vector<std::pair<int, std::string> >				_error_log;
std::vector<std::pair<int, std::string> >				_access_log;
std::vector<std::pair<int, std::string> >				_error_page;
std::vector<std::pair<int, int> >						_limit_request;
std::vector<std::pair<int, std::list<std::string> > >	_method_lists;
std::vector<std::pair<int, std::string> >				_cgi_php;

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
	_error_log.push_back(std::pair<int, std::string>(error_log.first, error_log.second.front()));
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
	_access_log.push_back(std::pair<int, std::string>(access_log.first, access_log.second.front()));
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

#endif
