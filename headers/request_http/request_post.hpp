#ifndef __REQUEST_POST_HPP__
# define __REQUEST_POST_HPP__


std::string	get_request_post_boudary( std::string &content_type ) {
	int			pos = content_type.find("boundary=");
	if (pos == -1)
		Error_exception::error(_parser.get_file_bad_request(), 400);
	pos += strlen("boundary=");
	std::string	boundary = "--" + content_type.substr(pos, \
		content_type.find_first_of("\r\n") - pos);
	return (boundary);
}


void	set_request_post_data( const std::string &host,
	std::string &tmp, const std::string &boundary ) {
	tmp = tmp.substr(tmp.find(boundary) + boundary.size());
	if (tmp == "--\r\n")
		return ;
	int pos = tmp.find("filename=\"");
	if (pos == -1)
		Error_exception::error(_parser.get_file_bad_request(), 400);
	pos += strlen("filename=\"");
	std::string 	filename = tmp.substr(pos, tmp.find_first_of("\"\r\n", pos) - pos);
	if (!host.empty())
		filename = host + '_' + filename;
	std::ofstream	img(( _parser.get_file_save(0) + filename).c_str(), std::ios::out | std::ofstream::binary);
	if (img.is_open() == false)
		Error_exception::error(_parser.get_file_internal_server_error(), 500);
	pos = tmp.find("\r\n\r\n") + 4;
	std::string		data = tmp.substr(pos, tmp.find(boundary) - pos - 2);
	img.write(data.c_str(), data.size());
	img.close();
	set_request_post_data(host, tmp, boundary);
}


void	get_request_post( std::map<std::string, std::string> &request ) {
if (!request.count("Request-Content") || !request["Request-Content"].size())
		Error_exception::error(_parser.get_file_bad_request(), 400);
	if (!request.count("Content-Type:"))
		Error_exception::error(_parser.get_file_bad_request(), 400);
	std::string	boundary = get_request_post_boudary(request["Content-Type:"]);
	request.count("Host:") \
		? set_request_post_data(request["Host:"], request["Request-Content"], boundary) \
		: set_request_post_data(std::string(), request["Request-Content"], boundary);
	ret_request_http(request, _parser.get_file_created(), 201);
	return ;
}


# endif
