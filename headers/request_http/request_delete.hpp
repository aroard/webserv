#ifndef __REQUEST_DELETE_HPP__
# define __REQUEST_DELETE_HPP__


void	get_request_delete( void ) const {
	check_cookie_id(0);
	if (!_request.count("POST"))
		Error_exception::error(_parser.get_file_bad_request(), 400);
	const size_t pos = _request.at("POST").find_first_of(" \r\n");
	if (pos == std::string::npos)
		Error_exception::error(_parser.get_file_bad_request(), 400);
	std::string	path_file = _request.at("POST").substr(0, pos);
	path_file = _parser.get_root(0) + urldecode(path_file);
	if (std::remove(path_file.c_str()) != 0)
		Error_exception::error(_parser.get_file_not_found(), 404);
	else
	{
		std::string redirection = _parser.get_file_deleted();
		std::stringstream ss;
		ss << _port;
		std::string port_str = ss.str();
		if (redirection.find("PORT") != std::string::npos)
			redirection.replace(redirection.find("PORT"), 4, port_str);
		ret_request_http(redirection, 201);
	}
	return ;
}


#endif
