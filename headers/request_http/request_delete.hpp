#ifndef __REQUEST_DELETE_HPP__
# define __REQUEST_DELETE_HPP__


void	get_request_delete( std::map<std::string, std::string> &request, std::string root) {
	if (!request.count("POST"))
		Error_exception::error(_parser.get_file_bad_request(), 400);
	int pos = request["POST"].find_first_of(" \r\n");
	if (pos == -1)
		Error_exception::error(_parser.get_file_bad_request(), 400);
	std::string	path_file = request["POST"].substr(0, pos);
	path_file = root + urldecode(path_file);
	if (std::remove(path_file.c_str()) != 0)
		Error_exception::error(_parser.get_file_not_found(), 404);
	else
	{
		std::string redirection = _parser.get_file_created();
		//A REFAIRE
		std::string port_str = "8000";
		redirection.replace(redirection.find("port"), 4, port_str);
		ret_request_http(request, redirection, 201);
	}
	return ;
}

#endif