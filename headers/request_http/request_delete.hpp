#ifndef __REQUEST_DELETE_HPP__
# define __REQUEST_DELETE_HPP__

std::string replace_redirection(std::string file) const{
	int			nbs_of_slash = 0;
	std::string path = _parser.get_file_save(0);
	std::string link = "../delete.php";
	std::string redirection = file;
	for (size_t a(0); a < path.size(); a++){
		if (path[a] == '/')
			nbs_of_slash++;
	}
	for (int b(nbs_of_slash); b > 3; b--){
		link = "../" + link;
	}
	if (redirection.find("LINK") != std::string::npos)
		redirection.replace(redirection.find("LINK"), 4, link);
	return redirection;
}

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
		ret_request_http(replace_redirection(_parser.get_file_deleted()), 201);
	return ;
}


#endif
