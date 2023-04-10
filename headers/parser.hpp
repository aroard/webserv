#ifndef __PARSER_HPP__
# define __PARSER_HPP__


class Parser
{
private:
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
		set_file_http();
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
		_file_created = o._file_created;
		_file_bad_request = o._file_bad_request;
		_file_unauthorized = o._file_unauthorized;
		_file_forbidden = o._file_forbidden;
		_file_not_found = o._file_not_found;
		_file_methode_not_allowed = o._file_methode_not_allowed;
		_file_internal_server_error = o._file_internal_server_error;
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
		new_parser._error_log.push_back(std::pair<int, std::pair<std::string, int> >(1, get_error_log(i)));
		new_parser._access_log.push_back(std::pair<int, std::pair<std::string, int> >(1, get_access_log(i)));
		new_parser._error_page.push_back(std::pair<int, std::string>(1, get_error_page(i)));
		new_parser._limit_request.push_back(std::pair<int, int>(1, get_limit_request(i)));
		new_parser._method_lists.push_back(std::pair<int, std::list<std::string> >(1, get_method_lists(i)));
		new_parser._cgi_php.push_back(std::pair<int, std::string>(1, get_cgi_php(i)));
		new_parser._file_created = _file_created;
		new_parser._file_bad_request = _file_bad_request;
		new_parser._file_unauthorized = _file_unauthorized;
		new_parser._file_forbidden = _file_forbidden;
		new_parser._file_not_found = _file_not_found;
		new_parser._file_methode_not_allowed = _file_methode_not_allowed;
		new_parser._file_internal_server_error = _file_internal_server_error;
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
		if (parse_methode_server()) {
			parse_all_parameters();
			parse_all_file();
		}
		else
			parse_all_parameters();
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

	# include "./parser_utils/parser_setters.hpp"

	int	count_line_in_file( const std::string &path ) {
		std::ifstream	file(path.c_str(), std::ifstream::in);
		int				count = 0;

		if (file.is_open())
			for (std::string tmp; std::getline(file, tmp);)
				++count;
		return (count);
	}

	void	parse_all_parameters( void ) {
		if (get_port(_nb_conf_serv - 1).empty()) {
			std::list<int> empty_list(1, 8080);
			std::pair<int, std::list<int> > new_pair = std::make_pair(_nb_conf_serv, empty_list);
			_port.push_back(new_pair);
		}
		else {
			std::list<int> ls = get_port(_nb_conf_serv - 1);
			for (std::list<int>::iterator it = ls.begin(); it != ls.end(); ++it)
				if(*it < 0 || *it > 65536){
					std::cerr << "Error: Invalid port: " << *it << std::endl;
					exit(1);
				}
			}
		if (get_server_name(_nb_conf_serv - 1).empty()) 
			_server_name.push_back(std::pair<int, std::string>(_nb_conf_serv, "dinopoulet.42.fr"));
		// if (get_root(_nb_conf_serv - 1).empty())
			// _root.push_back(std::pair<int, std::string>(_nb_conf_serv, "./www/"));
		if (get_index(_nb_conf_serv - 1).empty()) {
			std::list<std::string> empty_list;
			empty_list.push_back("index.html");
			empty_list.push_back("index.php");
			_index.push_back(std::pair<int, std::list<std::string> >(_nb_conf_serv, empty_list));
		}
		if (get_error_log(_nb_conf_serv - 1).first.empty())
			_error_log.push_back(std::pair<int, std::pair<std::string, int> >(_nb_conf_serv, \
				std::pair<std::string, int>("./logs/error.log", count_line_in_file("./logs/error.log"))));
		if (get_access_log(_nb_conf_serv - 1).first.empty())
			_access_log.push_back(std::pair<int, std::pair<std::string, int> >(_nb_conf_serv, \
				std::pair<std::string, int>("./logs/access.log", count_line_in_file("./logs/access.log"))));
		if (get_error_page(_nb_conf_serv - 1).empty())
			_error_page.push_back(std::pair<int, std::string>(_nb_conf_serv, "./tools/not_found.html"));
		if (_limit_request.empty())	
			_limit_request.push_back(std::pair<int, int>(_nb_conf_serv, 1024));
		else {			
			if (get_limit_request(_nb_conf_serv - 1) < 1024 || get_limit_request(_nb_conf_serv - 1) > 102400) {
				std::cerr << "Error: Limit request must be between 1024-4096" << std::endl;
				exit(1);
			} 
		}
		if (get_method_lists(_nb_conf_serv - 1).empty()) {
			std::list<std::string> empty_list;
			empty_list.push_back("GET");
			empty_list.push_back("POST");
			empty_list.push_back("DELETE");
			std::pair<int, std::list<std::string> > new_pair = std::make_pair(_nb_conf_serv, empty_list);
			_method_lists.push_back(new_pair);

		}
		{
			const char	*method[] = { "GET", "POST", "DELETE", "" };
			std::list<std::string> ls = get_method_lists(_nb_conf_serv - 1);
			for (std::list<std::string>::iterator it = ls.begin(); it != ls.end(); ++it) {
				int a;
				for (a = 0; strlen(method[a]); a++) {
					if (method[a] == *it)
						break;
				}		
				if (!strlen(method[a])) {
					std::cout << "Invalid method list:" << *it << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		}
		if (get_cgi_php(_nb_conf_serv - 1).empty())
			_cgi_php.push_back(std::pair<int, std::string>(_nb_conf_serv, "./cgi_bin/php-cgi"));
	}

public:

	# include "./parser_utils/parser_getters.hpp"

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
		{ std::pair<std::string, int> pr = p.get_error_log(i);
		std::cout << "Error_log[" << i << "]\t\t: " << pr.first << ": " << pr.second << std::endl;
		} { std::pair<std::string, int> pr = p.get_access_log(i);
		std::cout << "Access_log[" << i << "]\t\t: " << pr.first << ": " << pr.second << std::endl; }
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