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
		Error_exception::error(_st_ch_register_ret1, 400);
	if (!_data.add_formulaire(form["name"], form["username"], form["email"], form["password"], atoi(form["age"].c_str())))
		Error_exception::error(_st_ch_register_ret2, 400);
	ret_request_http(_st_ch_register_ret3, 201);
}


#endif
