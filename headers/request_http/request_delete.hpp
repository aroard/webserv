#ifndef __REQUEST_DELETE_HPP__
# define __REQUEST_DELETE_HPP__


void	get_request_delete( void ) const {
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
		std::string redirection = _parser.get_file_created();
		std::stringstream ss;
		ss << _port;
		std::string port_str = ss.str();
		redirection.replace(redirection.find("port"), 4, port_str);
		ret_request_http(redirection, 201);
	}
	return ;
}


bool	is_image(const std::string &type) {
	int			reversed;
	std::string	extension;

	for (reversed = type.size(); reversed >= 0; reversed--) {
		if (type[reversed] == '.')
			break;
	}

	if (reversed > 3) {
		extension = type.substr(reversed);
		for (int i = 0; !g_img[i].empty(); ++i) {
			if (extension == g_img[i])
				return true;
		}
	}
	return false;
}

bool	is_video(const std::string &type) {
	int			reversed;
	std::string	extension;

	for (reversed = type.size(); reversed >= 0; reversed--) {
		if (type[reversed] == '.')
			break;
	}

	if (reversed > 3) {
		extension = type.substr(reversed);
		for (int i = 0; !g_video[i].empty(); ++i) {
			if (extension == g_video[i])
				return true;
		}
	}
	return false;
}

std::string	create_redirect(std::string path)  {
	int				reversed = 0;
	std::string		msg;
	std::string		name = path;
	std::string		string_to_return = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
	std::ifstream	ifs;

	if(path.find("//") != std::string::npos)
			path.replace(path.find("//"), 2, "/");
	if (is_image(path) == true)
		ifs.open("./tools/template/downloadimg.html");
	else if (is_video(path) == true)
		ifs.open("./tools/template/downloadvid.html");
	else
		ifs.open("./tools/template/downloadelse.html");
	if (path.find("/www/upload/") != std::string::npos)
		path.replace(path.find("/www/upload/"), 12, "/");
	if (!ifs)
	{
		std::cerr << "Error Template" << std::endl;
		return string_to_return;
	}

	for (reversed = name.size(); reversed >= 0; reversed--) {
		if (name[reversed] == '/'){
			reversed++;
			break;
		}
	}
	name = name.substr(reversed);
	while(std::getline(ifs, msg, '\r')) {
		if (msg.find("TITLE_CONVERT") != std::string::npos)
			msg.replace(msg.find("TITLE_CONVERT"), 13, name);
		while (msg.find("LINK") != std::string::npos)
			msg.replace(msg.find("LINK"), 4, path);
		string_to_return += msg;
	}
	ifs.close();
	return string_to_return;
}


void	get_request_redirection( void )  {
	if (!_request.count("POST"))
		Error_exception::error(_parser.get_file_bad_request(), 400);
	const size_t pos = _request.at("POST").find_first_of(" \r\n");
	if (pos == std::string::npos)
		Error_exception::error(_parser.get_file_bad_request(), 400);
	std::string	path_file = _request.at("POST").substr(0, pos);
	path_file = urldecode(path_file);
	std::string redirection = create_redirect(_parser.get_root(0) + path_file);
	ret_request_http(redirection, 201);
	return ;
}

#endif