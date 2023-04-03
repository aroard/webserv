#ifndef __PARSER_HPP__
# define __PARSER_HPP__

class Parser
{
public:
	std::string		_path_conf;
	std::ifstream	_file_conf;
	std::string		_data_conf;
	int				_nb_conf_serv;

public:

	explicit Parser( void ) : _nb_conf_serv(0) {}

	explicit Parser(int argc, char **argv) : _nb_conf_serv(0) {
		if (argc == 1)
			_path_conf = "./conf/conf.conf";
		else if (argc == 2)
			_path_conf = argv[1];
		else {
			std::cerr << "Too many config file" << std::endl;
			exit(EXIT_FAILURE);
		}
		_file_conf.open(_path_conf.c_str());
		if (!_file_conf.is_open()) {
			perror(_path_conf.c_str());
			exit(EXIT_FAILURE);
		}
		_file_conf.seekg(0, _file_conf.end);
		int	length = _file_conf.tellg();
		_file_conf.seekg(0, _file_conf.beg);
		char *buffer = new char[length];
		_file_conf.read(buffer, length);
		_data_conf = buffer;
		parse_all_file();
	}

	Parser( const Parser &o ) {
		if (this != &o)
			*this = o;
	}

	~Parser () {
		if (_file_conf.is_open())
			_file_conf.close();
	}

	Parser&	operator=( const Parser &o) {
		_path_conf = o._path_conf; _file_conf.open(o._path_conf.c_str());
		_data_conf = o._data_conf; _nb_conf_serv = o._nb_conf_serv;
		_port = o._port;
		_server_name = o._server_name;
		_root = o._root;
		_index = o._index;
		_error_log = o._error_log;
		_access_log = o._access_log;
		_error_page = o._error_page;
		_limit_request = o._limit_request;
		_method_lists = o._method_lists;
		_cgi_php = o._cgi_php;
		return (*this);
	}

	Parser	operator[]( size_t i ) {
		Parser	new_parser;

		if (i >= _nb_conf_serv)
			return (new_parser);
		new_parser._nb_conf_serv = 1;
		new_parser._port.push_back(std::pair<int, std::list<int> >(1, get_port(i)));
		new_parser._server_name.push_back(std::pair<int, std::string>(1, get_server_name(i)));
		new_parser._root.push_back(std::pair<int, std::string>(1, get_root(i)));
		new_parser._index.push_back(std::pair<int, std::list<std::string> >(1, get_index(i)));
		new_parser._error_log.push_back(std::pair<int, std::string>(1, get_error_log(i)));
		new_parser._access_log.push_back(std::pair<int, std::string>(1, get_access_log(i)));
		new_parser._error_page.push_back(std::pair<int, std::string>(1, get_error_page(i)));
		new_parser._limit_request.push_back(std::pair<int, int>(1, get_limit_request(i)));
		new_parser._method_lists.push_back(std::pair<int, std::list<std::string> >(1, get_method_lists(i)));
		new_parser._cgi_php.push_back(std::pair<int, std::string>(1, get_cgi_php(i)));
		return (new_parser);
	}

private:

	bool	parse_logfiles( std::string file ) {
		std::ofstream ofs(file.c_str(), std::ofstream::out | std::ofstream::app);
		if (!ofs) {
			perror("Error: Can't open or create a log file");
			return (false);
		}
		ofs.close();
		return (true);
	}

	bool	parse_files( std::string file ) {
		std::ifstream ifs(file.c_str());
		if (!ifs) {
			perror(std::string(file).c_str());
			return (false);
		}
		ifs.close();
		return (true);
	}


	void	parse_all_file( void ) {
		if (!_nb_conf_serv++)
			parse_methode_server();
		parse_config_methode();
		if (parse_methode_server())
			parse_all_file();
		return ;
	}


	bool	parse_methode_server( void ) {
		int	pos = _data_conf.find("server");
		if (pos == -1) {
			if (_nb_conf_serv > 0)
				return (false);
			std::cerr << "No methode server in config file" 
				<< __FUNCTION__ << ": " << __LINE__ << std::endl;
			exit(EXIT_FAILURE);
		}
		_data_conf = _data_conf.substr(pos);
		pos = _data_conf.find("{");
		if (pos == -1 || pos != _data_conf.find_first_not_of("server ")) {
			if (_nb_conf_serv > 0)
				return (false);
			std::cerr << "Syntax error: " << __FUNCTION__
				<< ": " << __LINE__ << std::endl;
			exit(EXIT_FAILURE);
		}
		_data_conf = _data_conf.substr(pos);
		pos = _data_conf.find("}");
		if (pos == -1) {
			if (_nb_conf_serv > 0)
				return (false);
			std::cerr << "Syntax error: " << __FUNCTION__
				<< ": " << __LINE__ << std::endl;
			exit(EXIT_FAILURE);
		}
		_data_conf = _data_conf.substr(1);
		if (_data_conf.substr(1, pos - 1).find_first_not_of(" \t\n\r") == std::string::npos) {
			if (_nb_conf_serv > 0)
				return (false);
			std::cerr << "The config file server is empty" 
				<< __FUNCTION__ << ": " << __LINE__ << std::endl;
			exit(EXIT_FAILURE);
		}
		return (true);
	}


	void	parse_config_methode( void ) {
		int	pos = _data_conf.find_first_not_of(" \t\n\r");
		_data_conf = _data_conf.substr(pos);
		pos = _data_conf.find_first_of(" \t");
		if (pos == std::string::npos) {
			if (_nb_conf_serv > 0)
				return ;
			std::cerr << "Syntax error: " << __FUNCTION__
				<< ": " << __LINE__ << std::endl;
			exit(EXIT_FAILURE);
		}
		std::string	tmp = _data_conf.substr(0, pos);
		for (int i = 0;; ++i) {
			if (g_config_methode[i].empty()) {
				std::cerr << "Name methode in methode server invalid" 
					<< __FUNCTION__ << ": " << __LINE__ << std::endl;
				exit(EXIT_FAILURE);
			}
			if (!tmp.compare(g_config_methode[i])) {
				set_config_methode(tmp, i); break ;
			}
		}
		if((_data_conf = \
			_data_conf.substr(_data_conf.find(";") + 1)).find(";") \
			< _data_conf.find("}"))
			parse_config_methode();
		else {
			pos = _data_conf.find_first_not_of(" \t\n\r");
			if (_data_conf[pos] != '}') {
				std::cerr << "Syntax error: " << __FUNCTION__
				<< ": " << __LINE__ << std::endl;
				exit(EXIT_FAILURE);
			}
			_data_conf = _data_conf.substr(pos + 1);
		}
		return ;
	}

	void	too_more_config_method( std::string func ) {
		std::cerr << "Syntax error: " << __FUNCTION__
			<< ": " << __LINE__ << ": " << func << std::endl;
			exit(EXIT_FAILURE);
	}


	void	set_config_methode( std::string &tmp , int i) {
		switch (i) {
			case 0: if (_port.size() < _nb_conf_serv) set_listen(tmp); else too_more_config_method("set_listen"); break ;
			case 1: if (_server_name.size() < _nb_conf_serv) set_server_name(tmp); else too_more_config_method("set_server_name"); break ;
			case 2: if (_root.size() < _nb_conf_serv) set_root(tmp); else too_more_config_method("set_root"); break ;
			case 3: if (_index.size() < _nb_conf_serv) set_index(tmp); else too_more_config_method("set_index"); break ;
			case 4: if (_error_log.size() < _nb_conf_serv) set_error_log(tmp); else too_more_config_method("set_error_log"); break ;
			case 5: if (_access_log.size() < _nb_conf_serv) set_access_log(tmp); else too_more_config_method("set_access_log"); break ;
			case 6: if (_error_page.size() < _nb_conf_serv) set_error_page(tmp); else too_more_config_method("set_error_page"); break ;
			case 7: if (_limit_request.size() < _nb_conf_serv) set_limit_request(tmp); else too_more_config_method("set_limit_request"); break ;
			case 8: if (_method_lists.size() < _nb_conf_serv) set_method_lists(tmp); else too_more_config_method("set_method_lists"); break ;
			case 9: if (_cgi_php.size() < _nb_conf_serv) set_cgi_php(tmp); else too_more_config_method("set_cgi_php"); break ;
			case 10: set_comment_line(tmp); break ;
		}
		return ;
	}

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
			if (it->find_first_not_of("-+0123456789") != -1) {
				std::cerr << "Syntax error: " << __FUNCTION__
					<< ": " << __LINE__ << std::endl;
				exit(EXIT_FAILURE);
			}
			ls.push_back(atoi(it->c_str()));
		}
		_port.push_back(std::pair<int, std::list<int> >(port.first, ls));
	}

bool	setting_server_name(std::string name)
{
	std::string host = "/etc/hosts";
	std::string host2 = "/etc/hosts.new";
    std::string line;
    std::ifstream ifs(host.c_str());
    std::ofstream ofs(host2.c_str(), std::ofstream::trunc);
    int i = 0;

    if(!ofs || !ifs || name.size() < 1)
    {
			if (!ofs)
				std::cout << "Error: ofs could not create" << std::endl;
			if (!ifs)
				std::cout << "Error: ifs could not open" << std::endl;
			if (name.size() < 1)
				std::cout << "Error: server name must be atleast 1 character" << std::endl;
			return false;
		}

		while(std::getline(ifs, line))
		{
			if (!i)
				ofs << line + " " + name << std::endl;
			else
				ofs << line << std::endl;
			i++;
		}

		ifs.close();
		ofs.close();

		if (std::remove(host.c_str()) != 0)
		{
			std::cout << "could not remove the folder :" + host << std::endl;
			return false;
		}
		if (std::rename(host2.c_str(), host.c_str()) != 0)
		{
			std::cout << "could not rename the folder :" + host2 << std::endl;
			return false;
		}
		return true;
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
		if (limit_request.second.size() != 1 || limit_request.second.front().find_first_not_of("-+0123456789") != -1) {
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

	std::vector<std::pair<int, std::list<int> > >			
	get_port( void ) { return (_port); }
	std::vector<std::pair<int, std::string> >	
	get_server_name( void ) { return (_server_name); }
	std::vector<std::pair<int, std::string> >	
	get_root( void ) { return (_root); }
	std::vector<std::pair<int, std::list<std::string> > >	
	get_index( void ) { return (_index); }
	std::vector<std::pair<int, std::string> >	
	get_error_log( void ) { return (_error_log); }
	std::vector<std::pair<int, std::string> >	
	get_access_log( void ) { return (_access_log); }
	std::vector<std::pair<int, std::string> >	
	get_error_page( void ) { return (_error_page); }
	std::vector<std::pair<int, int> >	
	get_limit_request( void ) { return (_limit_request); }
	std::vector<std::pair<int, std::list<std::string> > >	
	get_method_lists( void ) { return (_method_lists); }
	std::vector<std::pair<int, std::string> >	
	get_cgi_php( void ) { return (_cgi_php); }

	int				get_nb_conf_serv( void ) const { return (_nb_conf_serv); }
	std::list<int>	get_port( size_t num_conf ) const {
		return (_port.empty() ? std::list<int>() : num_conf < _port.back().first ? _port[num_conf].second :  std::list<int>()); 
	}
	std::string		get_server_name( size_t num_conf ) const {
		return (_server_name.empty() ? std::string() : num_conf < _server_name.back().first ? _server_name[num_conf].second : std::string()); 
	}
	std::string		get_root( size_t num_conf ) const {
		return (_root.empty() ? std::string() : num_conf < _root.back().first ? _root[num_conf].second : std::string()); 
	}
	std::list<std::string>		get_index( size_t num_conf ) const {
		return (_index.empty() ? std::list<std::string>() : num_conf < _index.back().first ? _index[num_conf].second : std::list<std::string>()); 
	}
	std::string		get_error_log( size_t num_conf ) const {
		return (_error_log.empty() ? std::string() : num_conf < _error_log.back().first ? _error_log[num_conf].second : std::string()); 
	}
	std::string		get_access_log( size_t num_conf ) const {
		return (_access_log.empty() ? std::string() : num_conf < _access_log.back().first ? _access_log[num_conf].second : std::string());
	}
	std::string		get_error_page( size_t num_conf ) const {
		return (_error_page.empty() ? std::string() : num_conf < _error_page.back().first ? _error_page[num_conf].second : std::string());
	}
	int				get_limit_request( size_t num_conf ) const {
		return (_limit_request.empty() ? int() : num_conf < _limit_request.back().first ? _limit_request[num_conf].second : int());
	}
	std::list<std::string>		get_method_lists( size_t num_conf ) const {
		return (_method_lists.empty() ? std::list<std::string>() : num_conf < _method_lists.back().first ? _method_lists[num_conf].second : std::list<std::string>());
	}
	std::string		get_cgi_php( size_t num_conf ) const {
		return (_cgi_php.empty() ? std::string() : num_conf < _cgi_php.back().first ? _cgi_php[num_conf].second : std::string());
	}

};

std::ostream&	operator<<( std::ostream &o, const Parser &p) {
	for (int i = 0; i < p.get_nb_conf_serv(); ++i) {
		std::cout << "---------------- CONFIG " << i << " ----------------" << std::endl;
		std::cout << "Port[" << i << "]\t\t\t: "; { std::list<int> ls = p.get_port(i);
		for (std::list<int>::iterator it = ls.begin(); it != ls.end(); ++it)
			std::cout << "(" << *it << ")"; std::cout << std::endl; }
		std::cout << "Server_name[" << i << "]\t\t: " << p.get_server_name(i) << std::endl;
		std::cout << "Root[" << i << "]\t\t\t: " << p.get_root(i) << std::endl;
		std::cout << "Index[" << i << "]\t\t: "; { std::list<std::string> ls = p.get_index(i);
		for (std::list<std::string>::iterator it = ls.begin(); it != ls.end(); ++it)
			std::cout << "(" << *it << ")"; std::cout << std::endl; }
		std::cout << "Error_log[" << i << "]\t\t: " << p.get_error_log(i) << std::endl;
		std::cout << "Access_log[" << i << "]\t\t: " << p.get_access_log(i) << std::endl;
		std::cout << "Error_page[" << i << "]\t\t: " << p.get_error_page(i) << std::endl;
		std::cout << "Limit_request[" << i << "]\t: " << p.get_limit_request(i) << std::endl;
		std::cout << "Method_request[" << i << "]\t: "; { std::list<std::string> ls = p.get_method_lists(i);
		for (std::list<std::string>::iterator it = ls.begin(); it != ls.end(); ++it)
			std::cout << "(" << *it << ")"; std::cout << std::endl; }
		std::cout << "Cgi_php[" << i << "]\t\t: " << p.get_cgi_php(i) << std::endl;
		std::cout << "------------------------------------------" << std::endl;
	}
	return o;
}

#endif