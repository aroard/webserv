#ifndef __PARSER_GETTERS_HPP__
# define __PARSER_GETTERS_HPP__

std::vector<std::pair<int, std::list<int> > >			get_port( void ) { return (_port); }
std::vector<std::pair<int, std::string> >				get_server_name( void ) { return (_server_name); }
std::vector<std::pair<int, std::string> >				get_root( void ) { return (_root); }
std::vector<std::pair<int, std::list<std::string> > >	get_index( void ) { return (_index); }
std::vector<std::pair<int, std::string> >				get_error_log( void ) { return (_error_log); }
std::vector<std::pair<int, std::string> >				get_access_log( void ) { return (_access_log); }
std::vector<std::pair<int, std::string> >				get_error_page( void ) { return (_error_page); }
std::vector<std::pair<int, int> >						get_limit_request( void ) { return (_limit_request); }
std::vector<std::pair<int, std::list<std::string> > >	get_method_lists( void ) { return (_method_lists); }
std::vector<std::pair<int, std::string> >				get_cgi_php( void ) { return (_cgi_php); }


int							get_nb_conf_serv( void ) const { return (_nb_conf_serv); }


std::list<int>				get_port( size_t num_conf ) const {
	return (_port.empty() ? std::list<int>() : num_conf < _port.back().first ? _port[num_conf].second :  std::list<int>()); 
}


std::string					get_server_name( size_t num_conf ) const {
	return (_server_name.empty() ? std::string() : num_conf < _server_name.back().first ? _server_name[num_conf].second : std::string()); 
}


std::string					get_root( size_t num_conf ) const {
	return (_root.empty() ? std::string() : num_conf < _root.back().first ? _root[num_conf].second : std::string()); 
}


std::list<std::string>		get_index( size_t num_conf ) const {
	return (_index.empty() ? std::list<std::string>() : num_conf < _index.back().first ? _index[num_conf].second : std::list<std::string>()); 
}


std::string					get_error_log( size_t num_conf ) const {
	return (_error_log.empty() ? std::string() : num_conf < _error_log.back().first ? _error_log[num_conf].second : std::string()); 
}


std::string					get_access_log( size_t num_conf ) const {
	return (_access_log.empty() ? std::string() : num_conf < _access_log.back().first ? _access_log[num_conf].second : std::string());
}


std::string					get_error_page( size_t num_conf ) const {
	return (_error_page.empty() ? std::string() : num_conf < _error_page.back().first ? _error_page[num_conf].second : std::string());
}


int							get_limit_request( size_t num_conf ) const {
	return (_limit_request.empty() ? int() : num_conf < _limit_request.back().first ? _limit_request[num_conf].second : int());
}


std::list<std::string>		get_method_lists( size_t num_conf ) const {
	return (_method_lists.empty() ? std::list<std::string>() : num_conf < _method_lists.back().first ? _method_lists[num_conf].second : std::list<std::string>());
}


std::string					get_cgi_php( size_t num_conf ) const {
	return (_cgi_php.empty() ? std::string() : num_conf < _cgi_php.back().first ? _cgi_php[num_conf].second : std::string());
}

#endif
