#ifndef __REQUEST_DELETE_HPP__
# define __REQUEST_DELETE_HPP__


void	get_request_delete( std::map<std::string, std::string> &request) {
	if (!request.count("POST"))
		Error_exception::error(_parser.get_file_bad_request(), 400);
	int pos = request["POST"].find_first_of(" \r\n");
	if (pos == -1)
		Error_exception::error(_parser.get_file_bad_request(), 400);
	std::string	path_file = request["POST"].substr(0, pos);
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
		put_line(redirection);
		ret_request_http(request, redirection, 201);
	}
	return ;
}

std::string	create_redirect(std::string path)
{
	std::cout << path << std::endl;
	std::string		string_to_return = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
	std::string		msg;
	std::ifstream	ifs;

	if(path.find("//") != std::string::npos)
			path.replace(path.find("//"), 2, "/");
	std::cout << path << std::endl;
	if (path.size() > 4 \
		&& (!path.compare(path.size() - 4, 4, ".jpg")\
			|| !path.compare(path.size() - 5, 5, ".jpeg")\
			|| !path.compare(path.size() - 4, 4, ".png")))
		ifs.open("./tools/template/downloadimg.html");
	else if (path.size() > 4 \
		&& !path.compare(path.size() - 4, 4, ".mp4"))
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

	std::cout << path << std::endl;
	while(std::getline(ifs, msg, '\r')){
		while (msg.find("LINK") != std::string::npos)
			msg.replace(msg.find("LINK"), 4, path);
		string_to_return += msg;
	}

	std::cout << path << std::endl;
	ifs.close();
	return string_to_return;
}


void	get_request_redirection( std::map<std::string, std::string> &request) {
	if (!request.count("POST"))
		Error_exception::error(_parser.get_file_bad_request(), 400);
	int pos = request["POST"].find_first_of(" \r\n");
	if (pos == -1)
		Error_exception::error(_parser.get_file_bad_request(), 400);
	std::string	path_file = request["POST"].substr(0, pos);
	path_file = urldecode(path_file);
	std::cout << _parser.get_root(0) << std::endl;
	std::string redirection = create_redirect(_parser.get_root(0) + path_file);
	put_line(redirection);
	ret_request_http(request, redirection, 201);
	return ;
}

#endif