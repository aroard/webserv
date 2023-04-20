#ifndef __REQUEST_REDIRECT_HPP__
# define __REQUEST_REDIRECT_HPP__


static const std::string	_st_str_img[];
static const std::string	_st_str_video[];



std::string replace_url(std::string file) const{
	int			nbs_of_slash = 0;
	std::string path = _parser.get_file_save(0);
	std::string link = "../index.html";
	std::string plus = "";
	std::string redirection = file;
	for (size_t a(0); a < path.size(); a++){
		if (path[a] == '/')
			nbs_of_slash++;
	}
	for (int b(nbs_of_slash); b > 3; b--){
		plus += "../";
	}
	if (redirection.find("../index.html") != std::string::npos)
		redirection.replace(redirection.find("../index.html"), link.size(), plus + link);
	link = "../upload.html";
	if (redirection.find("../upload.html") != std::string::npos)
		redirection.replace(redirection.find("../upload.html"), link.size(), plus + link);
	link = "../delete.php";
	if (redirection.find("../delete.php") != std::string::npos)
		redirection.replace(redirection.find("../delete.php"), link.size(), plus + link);
	link = "../aboutus.html";
	if (redirection.find("../aboutus.html") != std::string::npos)
		redirection.replace(redirection.find("../aboutus.html"), link.size(), plus + link);
	link = "../aboutus.html";
	if (redirection.find("../aboutus.html") != std::string::npos)
		redirection.replace(redirection.find("../aboutus.html"), link.size(), plus + link);
	return redirection;
}

bool	is_image( const std::string &type ) const {
	int			reversed;
	std::string	extension;

	for (reversed = type.size(); reversed >= 0; reversed--) {
		if (type[reversed] == '.')
			break;
	}

	if (reversed > 3) {
		extension = type.substr(reversed);
		for (int i = 0; !_st_str_img[i].empty(); ++i) {
			if (extension == _st_str_img[i])
				return (true);
		}
	}
	return (false);
}


bool	is_video( const std::string &type ) const {
	int			reversed;
	std::string	extension;

	for (reversed = type.size(); reversed >= 0; reversed--) {
		if (type[reversed] == '.')
			break;
	}

	if (reversed > 3) {
		extension = type.substr(reversed);
		for (int i = 0; !_st_str_video[i].empty(); ++i) {
			if (extension == _st_str_video[i])
				return (true);
		}
	}
	return (false);
}


std::string	create_redirect( std::string path ) const {
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
	if (!ifs) {
		std::cerr << "Error Template" << std::endl;
		return (string_to_return);
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
	string_to_return = replace_url(string_to_return);
	return (string_to_return);
}


void	get_request_redirect( void ) const {
	if (!_request.count("POST"))
		Error_exception::error(_parser.get_file_bad_request(), 400);
	const size_t pos = _request.at("POST").find_first_of(" \r\n");
	if (pos == std::string::npos)
		Error_exception::error(_parser.get_file_bad_request(), 400);
	std::string	path_file = _request.at("POST").substr(0, pos);
	path_file = urldecode(path_file);
	std::string redirection = create_redirect(path_file);
	ret_request_http(redirection, 201);
	return ;
}


#endif
