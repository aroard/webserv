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
		new_parser._port.push_back(std::pair<int, int>(1, get_port(i)));
		new_parser._server_name.push_back(std::pair<int, std::string>(1, get_server_name(i)));
		new_parser._root.push_back(std::pair<int, std::string>(1, get_root(i)));
		new_parser._index.push_back(std::pair<int, std::string>(1, get_index(i)));
		new_parser._error_log.push_back(std::pair<int, std::string>(1, get_error_log(i)));
		new_parser._access_log.push_back(std::pair<int, std::string>(1, get_access_log(i)));
		new_parser._error_page.push_back(std::pair<int, std::string>(1, get_error_page(i)));
		new_parser._limit_request.push_back(std::pair<int, std::string>(1, get_limit_request(i)));
		new_parser._method_lists.push_back(std::pair<int, std::string>(1, get_method_lists(i)));
		new_parser._cgi_php.push_back(std::pair<int, std::string>(1, get_cgi_php(i)));
		return (new_parser);
	}

private:

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

	void	set_config_methode( std::string &tmp , int i) {
		switch (i) {
			case 0: set_listen(tmp); break ;
			case 1: set_server_name(tmp); break ;
			case 2: set_root(tmp); break ;
			case 3: set_index(tmp); break ;
			case 4: set_error_log(tmp); break ;
			case 5: set_access_log(tmp); break ;
			case 6: set_error_page(tmp); break ;
			case 7: set_limit_request(tmp); break ;
			case 8: set_method_lists(tmp); break ;
			case 9: set_cgi_php(tmp); break ;
			case 10: set_comment_line(tmp); break ;
		}
		return ;
	}

	std::vector<std::pair<int, int> >			_port;
	std::vector<std::pair<int, std::string> >	_server_name;
	std::vector<std::pair<int, std::string> >	_root;
	std::vector<std::pair<int, std::string> >	_index;
	std::vector<std::pair<int, std::string> >	_error_log;
	std::vector<std::pair<int, std::string> >	_access_log;
	std::vector<std::pair<int, std::string> >	_error_page;
	std::vector<std::pair<int, std::string> >	_limit_request;
	std::vector<std::pair<int, std::string> >	_method_lists;
	std::vector<std::pair<int, std::string> >	_cgi_php;

	std::pair<int, std::string>	parse_template_set( std::string &tmp ) {
		int	pos = _data_conf.find(";") - tmp.size();
		if (pos == -1) {
			std::cerr << "Syntax error: " << __FUNCTION__
				<< ": " << __LINE__ << std::endl;
			exit(EXIT_FAILURE);
		}
		return (std::pair<int, std::string>(_nb_conf_serv, 
				_data_conf.substr(tmp.size() + 1, pos - 1)));
	}

	void	set_listen( std::string &tmp ) {
		std::pair<int, std::string> port = parse_template_set(tmp);
		if (port.second.find_first_not_of("-+0123456789") != -1) {
			std::cerr << "Syntax error: " << __FUNCTION__
				<< ": " << __LINE__ << std::endl;
			exit(EXIT_FAILURE);
		}
		_port.push_back(std::pair<int, int>(port.first, 
			atoi(port.second.c_str())));
		return ;
	}

	void	set_server_name( std::string &tmp ) {
		_server_name.push_back(parse_template_set(tmp));
		return ;
	}

	void	set_root( std::string &tmp ) {
		_root.push_back(parse_template_set(tmp));
		return ;
	}

	void	set_index( std::string &tmp ) {
		_index.push_back(parse_template_set(tmp));
		return ;
	}

	void	set_error_log( std::string &tmp ) {
		_error_log.push_back(parse_template_set(tmp));
		return ;
	}

	void	set_access_log( std::string &tmp ) {
		_access_log.push_back(parse_template_set(tmp));
		return ;
	}

	void	set_error_page( std::string &tmp ) {
		_error_page.push_back(parse_template_set(tmp));
		return ;
	}

	void	set_limit_request( std::string &tmp ) {
		_limit_request.push_back(parse_template_set(tmp));
		return ;
	}
	
	void	set_method_lists( std::string &tmp ) {
		_method_lists.push_back(parse_template_set(tmp));
		return ;
	}
	
	void	set_cgi_php( std::string &tmp ) {
		_cgi_php.push_back(parse_template_set(tmp));
		return ;
	}

	void	set_comment_line( std::string &tmp ) {
		_data_conf = _data_conf.substr(_data_conf.find_first_of(";"));
		return ;
	}

public:

	std::vector<std::pair<int, int> >			
	get_port( void ) { return (_port); }
	std::vector<std::pair<int, std::string> >	
	get_server_name( void ) { return (_server_name); }
	std::vector<std::pair<int, std::string> >	
	get_root( void ) { return (_root); }
	std::vector<std::pair<int, std::string> >	
	get_index( void ) { return (_index); }
	std::vector<std::pair<int, std::string> >	
	get_error_log( void ) { return (_error_log); }
	std::vector<std::pair<int, std::string> >	
	get_access_log( void ) { return (_access_log); }
	std::vector<std::pair<int, std::string> >	
	get_error_page( void ) { return (_error_page); }
	std::vector<std::pair<int, std::string> >	
	get_limit_request( void ) { return (_limit_request); }
	std::vector<std::pair<int, std::string> >	
	get_method_lists( void ) { return (_method_lists); }
	std::vector<std::pair<int, std::string> >	
	get_cgi_php( void ) { return (_cgi_php); }

	int				get_nb_conf_serv( void ) const { return (_nb_conf_serv); }
	int				get_port( size_t num_conf ) const {
		return (_port.empty() ? int() : num_conf < _port.back().first ? _port[num_conf].second : int()); 
	}
	std::string		get_server_name( size_t num_conf ) const {
		return (_server_name.empty() ? std::string() : num_conf < _server_name.back().first ? _server_name[num_conf].second : std::string()); 
	}
	std::string		get_root( size_t num_conf ) const {
		return (_root.empty() ? std::string() : num_conf < _root.back().first ? _root[num_conf].second : std::string()); 
	}
	std::string		get_index( size_t num_conf ) const {
		return (_index.empty() ? std::string() : num_conf < _index.back().first ? _index[num_conf].second : std::string()); 
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
	std::string		get_limit_request( size_t num_conf ) const {
		return (_limit_request.empty() ? std::string() : num_conf < _limit_request.back().first ? _limit_request[num_conf].second : std::string());
	}
	std::string		get_method_lists( size_t num_conf ) const {
		return (_method_lists.empty() ? std::string() : num_conf < _method_lists.back().first ? _method_lists[num_conf].second : std::string());
	}
	std::string		get_cgi_php( size_t num_conf ) const {
		return (_cgi_php.empty() ? std::string() : num_conf < _cgi_php.back().first ? _cgi_php[num_conf].second : std::string());
	}

};

std::ostream&	operator<<( std::ostream &o, const Parser &p) {
	for (int i = 0; i < p.get_nb_conf_serv(); ++i) {
		std::cout << "---------------- CONFIG " << i << " ----------------" << std::endl;
		std::cout << "Port[" << i << "]\t\t\t: " << p.get_port(i) << std::endl;
		std::cout << "Server_name[" << i << "]\t\t: " << p.get_server_name(i) << std::endl;
		std::cout << "Root[" << i << "]\t\t\t: " << p.get_root(i) << std::endl;
		std::cout << "Index[" << i << "]\t\t: " << p.get_index(i) << std::endl;
		std::cout << "Error_log[" << i << "]\t\t: " << p.get_error_log(i) << std::endl;
		std::cout << "Access_log[" << i << "]\t\t: " << p.get_access_log(i) << std::endl;
		std::cout << "Error_page[" << i << "]\t\t: " << p.get_error_page(i) << std::endl;
		std::cout << "Limit_request[" << i << "]\t: " << p.get_limit_request(i) << std::endl;
		std::cout << "Method_request[" << i << "]\t: " << p.get_method_lists(i) << std::endl;
		std::cout << "Cgi_php[" << i << "]\t\t: " << p.get_cgi_php(i) << std::endl;
		std::cout << "------------------------------------------" << std::endl;
	}
	return o;
}

#endif