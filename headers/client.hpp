#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__


class Client
{
private:
	std::map<std::string, std::string>	_request;
	const int							_socketClient;
	const int							_port;
	Parser								&_parser;

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
		if (msg.size() > _parser.get_body_limit(0) && _parser.get_body_limit(0) != 0)
			return (false);
		get_request_http(msg);
		// std::cout << "\033[32m████████████████ REQUEST ████████████████\033[0m" << std::endl;
		// for (std::map<std::string, std::string>::iterator it = _request.begin();
		// 	it != _request.end(); ++it) {
		// 	std::cout << "["; put_line(it->first); std::cout << "]";
		// 	std::cout << "("; put_line(it->second); std::cout << ")" << std::endl;
		// }
		// std::cout << "\033[32m█████████████████████████████████████████\033[0m" << std::endl;
		// std::cout << "\033[32m████████████████ MSG ████████████████\033[0m" << std::endl;
		// put_line(msg);
		// std::cout << "\033[32m█████████████████████████████████████████\033[0m" << std::endl;
		// std::cout << "msg:" << msg.size() << std::endl;
		return (true);
	}


	void	send_request_http( void ) {
		try {
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
		} catch (const Error_exception &e) {
			// std::cout << e.what().first << " et " << e.what().second << std::endl;
			ret_request_http(e.what().first, e.what().second); 
			return ;
		}
		ret_request_http(_parser.get_file_methode_not_allowed(), 405); 
		return ;
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
		std::stringstream	ss;
		std::time_t			t = std::time(NULL);
		std::tm 			*now = std::gmtime(&t);
			
		ss << std::setw(2) << "[" << now->tm_mday << "/";
		ss << g_month[now->tm_mon] << "/";
		ss << std::setw(2) << (now->tm_year + 1900) << ":";
	   	ss << std::setw(2) << std::setfill('0') << now->tm_hour << ":";
	   	ss << std::setw(2) << std::setfill('0') << now->tm_min << ":";
	   	ss << std::setw(2) << std::setfill('0') << now->tm_sec << " +0000]";
		return ss.str();
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


	void	ret_request_http( const std::string &msg, const int error_code ) const {

		signal(SIGPIPE, Error_exception::socket_close);
		const int	nb_octets = send(_socketClient, msg.c_str(), msg.size(), 0);
		signal(SIGPIPE, SIG_DFL);
		if (nb_octets < 1)
			throw Error_exception("Bad send", 1);
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
			log_file << error_code << " " << msg.size() << " \"-\" \"";
			if (_request.count("User-Agent:"))
				log_file << _request.at("User-Agent:");
			log_file << "\"\n";
			log_file.close();
		}
		return ;
	}


	std::string	urldecode(const std::string &str) const {
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


	std::string	get_cookie_id( int i ) const {
		if (i && !_request.count("Cookie:"))
			Error_exception::error("HTTP/1.1 201 Created\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
		<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./authenticated.html\">\r\n</head>\
		<body>\r\n<h1>User not login</h1>\r\n<section>\r\n<img src=./tools/Dinocry.png alt=Ma photo>\r\n</section>\r\n</body\r\n</html>\r\n", 201);
		else if (!i && !_request.count("Cookie:"))
			Error_exception::error("HTTP/1.1 201 Created\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
		<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=../authenticated.html\">\r\n</head>\
		<body>\r\n<h1>User not login</h1>\r\n<section>\r\n<img src=../tools/Dinocry.png alt=Ma photo>\r\n</section>\r\n</body\r\n</html>\r\n", 201);
		size_t	pos = _request.at("Cookie:").find("session_id=");
		if (pos == std::string::npos)
			Error_exception::error(_parser.get_file_bad_request(), 400);
		pos += 11;
		std::string	cookie_id = _request.at("Cookie:").substr(pos, \
			_request.at("Cookie:").find_first_of("\n\r") - pos);
		return (cookie_id);
	}


	void	check_cookie_id( int i ) const {
		std::string	cookie_id = get_cookie_id(i);
		if (!g_data.check_login(cookie_id))
			Error_exception::error("HTTP/1.1 201 Created\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
		<meta charset=\"UTF-8\">\r\n<meta http-equiv=\"refresh\" content=\"2; url=./authenticated.html\">\r\n</head>\
		<body>\r\n<h1>User not login</h1>\r\n<section>\r\n<img src=./tools/Dinocry.png alt=Ma photo>\r\n</section>\r\n</body\r\n</html>\r\n", 201);
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


#endif
