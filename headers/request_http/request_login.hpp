#ifndef __REQUEST_LOGIN_HPP__
# define __REQUEST_LOGIN_HPP__


std::string	generateRandomCookie( int length ) const{
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::string cookie;
	srand(time(NULL));
	for (int i = 0; i < length; ++i) {
		cookie += alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	return (cookie);
}


void	get_request_login( void ) const {

	if (!_request.count("Request-Content"))
		Error_exception::error(_parser.get_file_bad_request(), 400);
	const std::string	content = urldecode(_request.at("Request-Content"));
	std::map<std::string, std::string>	form;
	for (size_t	pos = 0, pos2; (pos = content.find("Login-", pos)) != std::string::npos;) {
		pos += 6;
		pos2 = content.find_first_of('=', pos) + 1;
		form[content.substr(pos, content.find_first_of('=', pos) - (pos))] \
			= content.substr(pos2, content.find_first_of("%%&\r\n", pos2) - pos2);
	}
	if (!form.count("username") || !form.count("password"))
		Error_exception::error(_st_ch_login_ret1, 400);
	
	if (!_data.check_login(form["username"]))
		Error_exception::error(_st_ch_login_ret2, 400);
	
	if (_data.get_alive(form["username"]))
		Error_exception::error(_st_ch_login_ret3, 400);
	
	if (form["password"].compare(_data.get_password(form["username"])))
		Error_exception::error(_st_ch_login_ret4, 400);
	
	_data.set_cookie(form["username"], generateRandomCookie(21));
	char	buffer[16];
	sprintf(buffer, "%d", _data.get_lifespan(form["username"]));
	ret_request_http("HTTP/1.1 201 Created\r\nContent-Type: text/html\r\nSet-Cookie: session_id=" \
		+ _data.get_cookie(form["username"]) + "; Max-Age=" + buffer + "\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./index.html\">\r\n</head>\
<body>\r\n<h1>User Login</h1>\r\n<section>\r\n<img src=./tools/Dinohappy2.png alt=Ma photo>\r\n</section></body\r\n</html>\r\n", 201);
}


#endif
