#ifndef __REQUEST_LOGOUT_HPP__
# define __REQUEST_LOGOUT_HPP__


void	get_request_logout( void ) const {
	std::string	cookie_id = get_cookie_id(1);
	if (!_data.check_login(cookie_id))
		Error_exception::error(_st_ch_logout_ret1, 400);
	_data.set_alive(cookie_id, false);

	ret_request_http("HTTP/1.1 201 Created\r\nContent-Type: text/html\r\nSet-Cookie: session_id=" \
+ cookie_id + "; Max-Age=-1\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./index.html\">\r\n</head>\
<body>\r\n<h1>User Logout</h1>\r\n</body\r\n</html>\r\n", 201);
	return ;
}


#endif
