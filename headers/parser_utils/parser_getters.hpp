#ifndef __PARSER_GETTERS_HPP__
# define __PARSER_GETTERS_HPP__


std::vector<std::pair<int, std::list<int> > >				get_port( void ) { return (_port); }
std::vector<std::pair<int, std::string> >					get_server_name( void ) { return (_server_name); }
std::vector<std::pair<int, std::string> >					get_root( void ) { return (_root); }
std::vector<std::pair<int, std::list<std::string> > >		get_index( void ) { return (_index); }
std::vector<std::pair<int, std::pair<std::string, int> > >	get_error_log( void ) { return (_error_log); }
std::vector<std::pair<int, std::pair<std::string, int> > >	get_access_log( void ) { return (_access_log); }
std::vector<std::pair<int, std::string> >					get_error_page( void ) { return (_error_page); }
std::vector<std::pair<int, int> >							get_limit_request( void ) { return (_limit_request); }
std::vector<std::pair<int, std::list<std::string> > >		get_method_lists( void ) { return (_method_lists); }
std::vector<std::pair<int, std::string> >					get_cgi_php( void ) { return (_cgi_php); }
std::vector<std::pair<int, std::string> >					get_file_save( void ) { return (_file_save); }
std::vector<std::pair<int, int> >							get_body_limit( void ) { return (_body_limit); }


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


std::pair<std::string, int>	get_error_log( size_t num_conf ) const {
	return (_error_log.empty() ? std::pair<std::string, int>() : num_conf < _error_log.back().first ? _error_log[num_conf].second : std::pair<std::string, int>()); 
}


std::pair<std::string, int>	get_access_log( size_t num_conf ) const {
	return (_access_log.empty() ? std::pair<std::string, int>() : num_conf < _access_log.back().first ? _access_log[num_conf].second : std::pair<std::string, int>());
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


std::string					get_file_save( size_t num_conf ) const {
	return (_file_save.empty() ? std::string() : num_conf < _file_save.back().first ? _file_save[num_conf].second : std::string());
}

int							get_body_limit( size_t num_conf ) const {
	return (_body_limit.empty() ? int() : num_conf < _body_limit.back().first ? _body_limit[num_conf].second : int());
}

std::string					get_file_created( void ) const { return (_file_created); }
std::string					get_file_bad_request( void ) const { return (_file_bad_request); }
std::string					get_file_unauthorized( void ) const { return (_file_unauthorized); }
std::string					get_file_forbidden( void ) const { return (_file_forbidden); }
std::string					get_file_not_found( void ) const { return (_file_not_found); }
std::string					get_file_methode_not_allowed( void ) const { return (_file_methode_not_allowed); }
std::string					get_file_internal_server_error( void ) const { return (_file_internal_server_error); }


#endif
