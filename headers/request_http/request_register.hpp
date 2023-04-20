#ifndef __REQUEST_REGISTER_HPP__
# define __REQUEST_REGISTER_HPP__


void	get_request_register( void ) const {

	if (!_request.count("Request-Content"))
		Error_exception::error(_parser.get_file_bad_request(), 400);
	const std::string	content = urldecode(_request.at("Request-Content"));
	std::map<std::string, std::string>	form;
	for (size_t	pos = 0, pos2; (pos = content.find("Register-", pos)) != std::string::npos;) {
		pos += 9;
		pos2 = content.find_first_of('=', pos) + 1;
		form[content.substr(pos, content.find_first_of('=', pos) - (pos))] \
			= content.substr(pos2, content.find_first_of("%%&\r\n", pos2) - pos2);
	}
	
	if (!form.count("name") || !form.count("username") || !form.count("email") || !form.count("password") || !form.count("age"))
		Error_exception::error("HTTP/1.1 400 Bad Request\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./register.html\">\r\n</head>\
<body>\r\n<h1>Error number of values...</h1>\r\n</body\r\n</html>\r\n", 400);
	
	if (!g_data.add_formulaire(form["name"], form["username"], form["email"], form["password"], atoi(form["age"].c_str())))
		Error_exception::error("HTTP/1.1 400 Bad Request\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./register.html\">\r\n</head>\
<body>\r\n<h1>User already created...<section>\r\n<img src=./tools/Dinocry.png alt=Ma photo>\r\n</section></h1>\r\n</body\r\n</html>\r\n", 400);

	ret_request_http("HTTP/1.1 201 Created\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./login.html\">\r\n</head>\
<body>\r\n<h1>New User Created</h1>\r\n<section>\r\n<img src=./tools/Dinohappy.png alt=Ma photo>\r\n</section></body\r\n</html>\r\n", 201);
}


#endif
