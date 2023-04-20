#ifndef __PARSER_GETTERS_HPP__
# define __PARSER_GETTERS_HPP__


std::vector<std::pair<size_t, std::list<size_t> > >					get_port( void ) const { return (_port); }
std::vector<std::pair<size_t, std::string> >						get_server_name( void ) const { return (_server_name); }
std::vector<std::pair<size_t, std::string> >						get_root( void ) const { return (_root); }
std::vector<std::pair<size_t, std::list<std::string> > >			get_index( void ) const { return (_index); }
std::vector<std::pair<size_t, std::pair<std::string, size_t> > >	get_error_log( void ) const { return (_error_log); }
std::vector<std::pair<size_t, std::pair<std::string, size_t> > >	get_access_log( void ) const { return (_access_log); }
std::vector<std::pair<size_t, std::string> >						get_error_page( void ) const { return (_error_page); }
std::vector<std::pair<size_t, size_t> >								get_limit_request( void ) const { return (_limit_request); }
std::vector<std::pair<size_t, std::list<std::string> > >			get_method_lists( void ) const { return (_method_lists); }
std::vector<std::pair<size_t, std::string> >						get_cgi_php( void ) const { return (_cgi_php); }
std::vector<std::pair<size_t, std::string> >						get_file_save( void ) const { return (_file_save); }
std::vector<std::pair<size_t, size_t> >								get_body_limit( void ) const { return (_body_limit); }
std::vector<std::pair<size_t, std::string> >						get_cgi_py( void ) const { return (_cgi_py); }


int							get_nb_conf_serv( void ) const { return (_nb_conf_serv); }


std::list<size_t>			get_port( const size_t num_conf ) const {
	return (_port.empty() ? std::list<size_t>() : num_conf < _port.back().first ? _port[num_conf].second :  std::list<size_t>()); 
}


std::string					get_server_name( const size_t num_conf ) const {
	return (_server_name.empty() ? std::string() : num_conf < _server_name.back().first ? _server_name[num_conf].second : std::string()); 
}


std::string					get_root( const size_t num_conf ) const {
	return (_root.empty() ? std::string() : num_conf < _root.back().first ? _root[num_conf].second : std::string()); 
}


std::list<std::string>		get_index( const size_t num_conf ) const {
	return (_index.empty() ? std::list<std::string>() : num_conf < _index.back().first ? _index[num_conf].second : std::list<std::string>()); 
}


std::pair<std::string, size_t>	get_error_log( const size_t num_conf ) const {
	return (_error_log.empty() ? std::pair<std::string, size_t>() : num_conf < _error_log.back().first ? _error_log[num_conf].second : std::pair<std::string, size_t>()); 
}


std::pair<std::string, size_t>	get_access_log( const size_t num_conf ) const {
	return (_access_log.empty() ? std::pair<std::string, size_t>() : num_conf < _access_log.back().first ? _access_log[num_conf].second : std::pair<std::string, size_t>());
}


std::string					get_error_page( const size_t num_conf ) const {
	return (_error_page.empty() ? std::string() : num_conf < _error_page.back().first ? _error_page[num_conf].second : std::string());
}


size_t							get_limit_request( const size_t num_conf ) const {
	return (_limit_request.empty() ? size_t() : num_conf < _limit_request.back().first ? _limit_request[num_conf].second : size_t());
}


std::list<std::string>		get_method_lists( const size_t num_conf ) const {
	return (_method_lists.empty() ? std::list<std::string>() : num_conf < _method_lists.back().first ? _method_lists[num_conf].second : std::list<std::string>());
}


std::string					get_cgi_php( const size_t num_conf ) const {
	return (_cgi_php.empty() ? std::string() : num_conf < _cgi_php.back().first ? _cgi_php[num_conf].second : std::string());
}


std::string					get_cgi_py( const size_t num_conf ) const {
	return (_cgi_py.empty() ? std::string() : num_conf < _cgi_py.back().first ? _cgi_py[num_conf].second : std::string());
}


std::string					get_file_save( const size_t num_conf ) const {
	return (_file_save.empty() ? std::string() : num_conf < _file_save.back().first ? _file_save[num_conf].second : std::string());
}

size_t							get_body_limit( const size_t num_conf ) const {
	return (_body_limit.empty() ? size_t() : num_conf < _body_limit.back().first ? _body_limit[num_conf].second : size_t());
}


const std::string&			get_file_created( void ) const { return (_file_created); }
const std::string&			get_file_deleted( void ) const { return (_file_deleted); }
const std::string&			get_file_bad_request( void ) const { return (_file_bad_request); }
const std::string&			get_file_unauthorized( void ) const { return (_file_unauthorized); }
const std::string&			get_file_forbidden( void ) const { return (_file_forbidden); }
const std::string&			get_file_not_found( void ) const { return (_file_not_found); }
const std::string&			get_file_methode_not_allowed( void ) const { return (_file_methode_not_allowed); }
const std::string&			get_file_body_limit( void ) const { return (_file_body_limit); }
const std::string&			get_file_internal_server_error( void ) const { return (_file_internal_server_error); }
const std::string&			get_file_authenticated( void ) const { return (_file_authenticated); }
const std::string&			get_file_register( void ) const { return (_file_register); }
const std::string&			get_file_login( void ) const { return (_file_login); }


#endif
