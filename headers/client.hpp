#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__


class Client
{
private:
	std::map<std::string, std::string>	_request;
	size_t								_size_request;
	const int							_socketClient;
	const int							_port;
	Parser								&_parser;

	static Data							_data;

	Client( void );

public:
	explicit Client(const int socketServer, const int port, Parser &parser) : _socketClient(accept(socketServer, NULL, NULL)),
	_port(port), _parser(parser) {
		if (_socketClient < 0) {
			perror("ERRPR accept"); exit(EXIT_FAILURE);
		}
	}

	~Client() {
		close (_socketClient);
	}

	int		get_port( void ) const { return (_port); }
	int		get_socket_client( void ) const { return (_socketClient); }


	bool	recv_request_http( void ) {
		std::string							msg;
		char								*tmp = \
				new char[_parser.get_limit_request(0) + 1];
		int 								nb_octets;
		size_t								pos = std::string::npos;

		std::memset(tmp, 0, _parser.get_limit_request(0));
		while (true) {
			nb_octets = recv(_socketClient, tmp, _parser.get_limit_request(0), 0);
			if (nb_octets < 1) {
				delete[] tmp;
				return (false);
			}
			msg.insert(msg.size(), tmp, nb_octets);
			pos = msg.find("Content-Length: ");
			if (pos != std::string::npos) {
				pos += 16;
				pos = atoi(msg.substr(pos, (msg.substr(pos)).find_first_of("\r\n")).c_str());
				nb_octets = msg.find("\r\n\r\n");
				if (nb_octets != -1) pos += nb_octets + 4;
				else pos = 1 << 30;
			}
			else if (msg.find("\r\n\r\n") != std::string::npos)
				break ;
			std::memset(tmp, 0, _parser.get_limit_request(0));
			if (pos != std::string::npos && msg.size() >= pos)
				break ;
		}
		delete[] tmp;
		_size_request = msg.size();
		get_request_http(msg);
		return (true);
	}


	void	send_request_http( void ) const {
		try {
			const size_t	body_limit = _parser.get_body_limit(0);	
			if (body_limit == 0 || body_limit > _size_request) {
				std::list<std::string>	ls = _parser.get_method_lists(0);
				for (std::list<std::string>::const_iterator it = ls.begin();
					it != ls.end(); ++it) {
					if (_request.count(*it)) {
						if (_request.at("Methode-http:") == "GET") get_request_get();
						else if (_request.at("Methode-http:") == "POST") get_request_post();
						else if (_request.at("Methode-http:") == "DELETE") get_request_delete();
						else if (_request.at("Methode-http:") == "REDIRECT") get_request_redirect();
						else if (_request.at("Methode-http:") == "REGISTER") get_request_register();
						else if (_request.at("Methode-http:") == "LOGIN") get_request_login();
						else if (_request.at("Methode-http:") == "LOGOUT") get_request_logout();
						else ret_request_http(_parser.get_file_methode_not_allowed(), 405);
						return ;
					}
				}
			}
			else
				ret_request_http(_parser.get_file_body_limit(), 413);
		} catch (const Error_exception &e) {
			try {
				ret_request_http(e.what().first, e.what().second);
			}
			catch (const Error_exception &e) {
				print_in_log_file(e.what().first.size(), e.what().second, e.what().first.c_str());
			}
			return ;
		}
		try {
			ret_request_http(_parser.get_file_methode_not_allowed(), 405);
		}
		catch (const Error_exception &e) {
			print_in_log_file(e.what().first.size(), e.what().second, e.what().first.c_str());	
		}
	}

private:

	void	get_request_http( const std::string &msg ) {
		std::stringstream	ss(msg);
		std::string			line;
		size_t				pos;

		pos = msg.find_first_of(' ');
		if (pos != std::string::npos)
			_request["Methode-http:"] = msg.substr(0, pos);
		while (std::getline(ss, line)) {
			line += '\n';
			if (line.find("\r\n") == 0) {
				while (std::getline(ss, line)) {
					_request["Request-Content"] += line + '\n';
				}
				pos = _request["Request-Content"].find("Methode-http=");
				if (pos != std::string::npos) { pos += 13;
					_request["Methode-http:"] = _request["Request-Content"].substr(\
						pos, _request["Request-Content"].find_first_of("%%&\r\n", pos) - pos);
				}
				return ;
			}
			pos = line.find_first_of(' ');
			if (pos == std::string::npos)
				continue ;
			_request[line.substr(0, pos)] = line.substr(pos + 1, \
				line.find("\r\n") - (pos + 1));
		}
		return ;
	}


	std::string	getDateAndTime( void ) const {
		static const std::string	month[] = 
					{ "Jan", "Feb", "Mar", "Apr", \
					  "May", "Jun", "Jul", "Aug", \
					  "Sep", "Oct", "Nov", "Dec" };
		std::stringstream	ss;
		std::time_t			t = std::time(NULL);
		std::tm 			*now = std::gmtime(&t);
			
		ss << std::setw(2) << "[" << now->tm_mday << "/";
		ss << month[now->tm_mon] << "/";
		ss << std::setw(2) << (now->tm_year + 1900) << ":";
	   	ss << std::setw(2) << std::setfill('0') << now->tm_hour << ":";
	   	ss << std::setw(2) << std::setfill('0') << now->tm_min << ":";
	   	ss << std::setw(2) << std::setfill('0') << now->tm_sec << " +0000]";
		return (ss.str());
	}


	int	trunc_log_file( const std::string &path ) const {
		std::ifstream	input_file(path.c_str(), std::ifstream::in);
		std::string		line_file;
		std::string		data_file;
		int 			nb_line = 0;

		if (input_file.is_open()) {
			for (int count = 0; std::getline(input_file, line_file); ++count) {
				if (count >= 30) {
					++nb_line;
					data_file += line_file + '\n';
				}
			}
			input_file.close();
		}
		std::ofstream	output_file(path.c_str(), std::ofstream::out | std::ofstream::trunc);
		if (output_file.is_open()) {
			output_file.write(data_file.c_str(), data_file.size());
			output_file.close();
		}
		return (nb_line);
	}


	void	get_log_file( std::ofstream &log_file, const int &error_code ) const {

		if (error_code == 200 || error_code == 201) {
			std::string path = _parser.get_access_log(0).first;
			int			nb_line = _parser.get_access_log(0).second;
			if (++nb_line > 50)
				nb_line = trunc_log_file(path);
			_parser.set_line_access_log(0, nb_line);
			log_file.open(path.c_str(), \
				std::ofstream::out | std::ofstream::app);
		}
		else {
			std::string path = _parser.get_error_log(0).first;
			int			nb_line = _parser.get_error_log(0).second;
			if (++nb_line > 50)
				nb_line = trunc_log_file(path);
			_parser.set_line_error_log(0, nb_line);
			log_file.open(path.c_str(), \
				std::ofstream::out | std::ofstream::app);
		}
		if (!log_file.is_open())
			Error_exception::error(_parser.get_file_internal_server_error(), 500);
		return ;
	}


	void	print_in_log_file( const int msg_size, const int error_code, const char *info ) const {
		std::ofstream	log_file;
		get_log_file(log_file, error_code);
		if (log_file.is_open()) {
			if (_request.count("Host:"))
				log_file << _request.at("Host:");
			log_file << " - -"<< getDateAndTime() << ' ';
			if (_request.count("Methode-http:")) {
				log_file << _request.at("Methode-http:") + ' ';
				if (_request.count(_request.at("Methode-http:")))
					log_file << _request.at(_request.at("Methode-http:")) \
						<< ' ' << _request.at(_request.at("Methode-http:")) << "\" ";
			}
			else
				log_file << " \" ";
			log_file << error_code << " " << msg_size;
			if (info)
				log_file << " (" << info << ")";
			log_file << " \"-\" \"";
			if (_request.count("User-Agent:"))
				log_file << _request.at("User-Agent:");
			log_file << "\"\n";
			log_file.close();
		}
	}


	void	ret_request_http( const std::string &msg, const int error_code ) const {

		signal(SIGPIPE, Error_exception::socket_close);
		const int	nb_octets = send(_socketClient, msg.c_str(), msg.size(), 0);
		signal(SIGPIPE, SIG_DFL);
		if (nb_octets < 1)
			throw Error_exception("Bad send", 1);
		print_in_log_file(msg.size(), error_code, NULL);
		return ;
	}


	std::string	urldecode( const std::string &str ) const {
		std::string	result;
		char		ch;
		int			hexCode;
		for(size_t i = 0; i < str.length(); i++) {
			if(str[i] == '+')
				result += ' ';
			else if(str[i] == '%') {
				sscanf(str.substr(i + 1, 2).c_str(), "%x", &hexCode);
				ch = static_cast<char>(hexCode);
				result += ch;
				i += 2;
			}
			else
				result += str[i];
		}
		return (result);
	}


	static const char	*_st_ch_ret1;
	static const char	*_st_ch_ret2;

	static const char	*_st_ch_login_ret1;
	static const char	*_st_ch_login_ret2;
	static const char	*_st_ch_login_ret3;
	static const char	*_st_ch_login_ret4;

	static const char	*_st_ch_logout_ret1;

	static const char	*_st_ch_register_ret1;
	static const char	*_st_ch_register_ret2;
	static const char	*_st_ch_register_ret3;


	std::string	get_cookie_id( const int i ) const {
		if (i && !_request.count("Cookie:"))
			Error_exception::error(_st_ch_ret1, 201);
		else if (!i && !_request.count("Cookie:"))
			Error_exception::error(_st_ch_ret2, 201);
		size_t	pos = _request.at("Cookie:").find("session_id=");
		if (pos == std::string::npos)
			Error_exception::error(_parser.get_file_bad_request(), 400);
		pos += 11;
		std::string	cookie_id = _request.at("Cookie:").substr(pos, \
			_request.at("Cookie:").find_first_of("\n\r") - pos);
		return (cookie_id);
	}


	void	check_cookie_id( const int i ) const {
		std::string	cookie_id = get_cookie_id(i);
		if (!_data.check_login(cookie_id))
			Error_exception::error(_st_ch_ret1, 201);
		return ;
	}


	# include "./request_http/request_get.hpp"

	# include "./request_http/request_post.hpp"

	# include "./request_http/request_delete.hpp"

	# include "./request_http/request_redirect.hpp"

	# include "./request_http/request_register.hpp"

	# include "./request_http/request_login.hpp"

	# include "./request_http/request_logout.hpp"

};

Data	Client::_data;

const char	*Client::_st_ch_ret1 = \
"HTTP/1.1 201 Created\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./authenticated.html\">\r\n</head>\
<body>\r\n<h1>User not login</h1>\r\n<section>\r\n<img src=./tools/Dinocry.png alt=Ma photo>\r\n</section>\r\n</body\r\n</html>\r\n";

const char	*Client::_st_ch_ret2 = \
"HTTP/1.1 201 Created\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=../authenticated.html\">\r\n</head>\
<body>\r\n<h1>User not login</h1>\r\n<section>\r\n<img src=../tools/Dinocry.png alt=Ma photo>\r\n</section>\r\n</body\r\n</html>\r\n";

const char	*Client::_st_ch_login_ret1 = \
"HTTP/1.1 400 Bad Request\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./login.html\">\r\n</head>\
<body>\r\n<h1>Error number of values...</h1>\r\n</body\r\n</html>\r\n";

const char	*Client::_st_ch_login_ret2 = \
"HTTP/1.1 400 Bad Request\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./login.html\">\r\n</head>\
<body>\r\n<h1>Invalid login...</h1>\r\n<section>\r\n<img src=./tools/Dinocry.png alt=Ma photo>\r\n</section></body\r\n</html>\r\n";

const char	*Client::_st_ch_login_ret3 = \
"HTTP/1.1 400 Bad Request\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./authenticated.html\">\r\n</head>\
<body>\r\n<h1>Already login...</h1>\r\n<section>\r\n<img src=./tools/Dinocry.png alt=Ma photo>\r\n</section></body\r\n</html>\r\n";

const char	*Client::_st_ch_login_ret4 = \
"HTTP/1.1 400 Bad Request\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./login.html\">\r\n</head>\
<body>\r\n<h1>Invalid password...</h1>\r\n<<section>\r\n<img src=./tools/Dinocry.png alt=Ma photo>\r\n</section>/body\r\n</html>\r\n";

const char	*Client::_st_ch_logout_ret1 = \
"HTTP/1.1 201 Created\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./login.html\">\r\n</head>\
<body>\r\n<h1>Error invalid cookie_id</h1>\r\n</body\r\n</html>\r\n";

const char	*Client::_st_ch_register_ret1 = \
"HTTP/1.1 400 Bad Request\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./register.html\">\r\n</head>\
<body>\r\n<h1>Error number of values...</h1>\r\n</body\r\n</html>\r\n";

const char	*Client::_st_ch_register_ret2 = \
"HTTP/1.1 400 Bad Request\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./register.html\">\r\n</head>\
<body>\r\n<h1>User already created...<section>\r\n<img src=./tools/Dinocry.png alt=Ma photo>\r\n</section></h1>\r\n</body\r\n</html>\r\n";

const char	*Client::_st_ch_register_ret3 = \
"HTTP/1.1 201 Created\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./login.html\">\r\n</head>\
<body>\r\n<h1>New User Created</h1>\r\n<section>\r\n<img src=./tools/Dinohappy.png alt=Ma photo>\r\n</section></body\r\n</html>\r\n";


#endif
