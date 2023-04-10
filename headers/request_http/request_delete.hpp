#ifndef __REQUEST_DELETE_HPP__
# define __REQUEST_DELETE_HPP__


void	get_request_delete( std::map<std::string, std::string> &request ) {
	if (!request.count("POST"))
		Error_exception::error(_parser.get_file_bad_request(), 400);
	int pos = request["POST"].find_first_of(" \r\n");
	if (pos == -1)
		Error_exception::error(_parser.get_file_bad_request(), 400);
	std::string	path_file = request["POST"].substr(0, pos);
	path_file = urldecode(path_file);
	if (std::remove(path_file.c_str()) != 0)
		Error_exception::error(_parser.get_file_not_found(), 404);
	else
		ret_request_http(request, _parser.get_file_created(), 201); 
	return ;
}

#endif