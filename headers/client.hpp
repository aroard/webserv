#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__


class Client
{
private:
	std::map<std::string, std::string>	request;
	int									_socketClient;
	int									_port;
	Parser								&_parser;

	Client( void );

public:
	explicit Client(int socketServer, int port, Parser &parser) : _port(port), _parser(parser) {
		if ((_socketClient = accept(socketServer, NULL, NULL)) < 0) {
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
		int									pos = -1;

		std::memset(tmp, 0, _parser.get_limit_request(0));
		while ((nb_octets = recv(_socketClient, tmp, _parser.get_limit_request(0), 0)) != 0) {
			if (nb_octets == -1) {
				delete[] tmp;
				return (false);
			}
			msg.insert(msg.size(), tmp, nb_octets);
			pos = msg.find("Content-Length: ");
			if (pos != -1) {
				pos += 16;
				pos = atoi(msg.substr(pos, (msg.substr(pos)).find_first_of("\r\n")).c_str());
				nb_octets = msg.find("\r\n\r\n");
				if (nb_octets != -1) pos += nb_octets + 4;
				else pos = 1 << 30;
			}
			else if (msg.find("\r\n\r\n") != -1)
				break ;
			std::memset(tmp, 0, _parser.get_limit_request(0));
			if (pos != -1 && msg.size() >= pos)
				break ;
		}		
		if (msg.size() > _parser.get_body_limit(0) && _parser.get_body_limit(0) != 0)
			return false;
		get_request_http(request, msg);
		// std::cout << "\033[32m████████████████ REQUEST ████████████████\033[0m" << std::endl;
		// for (std::map<std::string, std::string>::iterator it = request.begin();
		// 	it != request.end(); ++it) {
		// 	std::cout << "["; put_line(it->first); std::cout << "]";
		// 	std::cout << "("; put_line(it->second); std::cout << ")" << std::endl;
		// }
		// std::cout << "\033[32m█████████████████████████████████████████\033[0m" << std::endl;
		// std::cout << "\033[32m████████████████ MSG ████████████████\033[0m" << std::endl;
		// put_line(msg);
		// std::cout << "\033[32m█████████████████████████████████████████\033[0m" << std::endl;
		// std::cout << "msg:" << msg.size() << std::endl;
		delete[] tmp;
		return (true);
	}

	void	send_request_http( void ) {
		try {
			std::list<std::string>	ls = _parser.get_method_lists(0);
			for (std::list<std::string>::iterator it = ls.begin();
				it != ls.end(); ++it) {
				if (request.count(*it)) {
					if (request["Methode-http:"] == "GET") get_request_get(request);
					else if (request["Methode-http:"] == "POST") get_request_post(request);
					else if (request["Methode-http:"] == "DELETE") get_request_delete(request);
					else if (request["Methode-http:"] == "REDIRECT") get_request_redirection(request);
					else ret_request_http(request, _parser.get_file_methode_not_allowed(), 405); 
					return ;
				}
			}
		} catch (const Error_exception &e) { 
			ret_request_http(request, e.what().first, e.what().second); 
		}
		ret_request_http(request, _parser.get_file_methode_not_allowed(), 405); 
		return ;
	}

private:

	void	get_request_http( std::map<std::string, std::string> &request, const std::string &msg ) {		
		std::stringstream	ss(msg);
		std::string			line;
		int					pos;

		pos = msg.find_first_of(' ');
		if (pos != -1)
			request["Methode-http:"] = msg.substr(0, pos);
		while (std::getline(ss, line)) {
			line += '\n';
			if (line.find("\r\n") == 0) {
				while (std::getline(ss, line)) {
					request["Request-Content"] += line + '\n';
				}
				pos = request["Request-Content"].find("Methode-http=");
				if (pos != -1) { pos += 13;
					request["Methode-http:"] = request["Request-Content"].substr(\
						pos, request["Request-Content"].find_first_of("%%&\r\n", pos) - pos);
				}
				return ;
			}
			pos = line.find_first_of(' ');
			if (pos == -1)
				continue ;
			request[line.substr(0, pos)] = line.substr(pos + 1, \
				line.find("\r\n") - (pos + 1));
		}
		return ;
	}


	std::string	getDateAndTime( void ) {
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


	int	trunc_log_file( const std::string &path ) {
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


	void	get_log_file( std::ofstream &log_file, const int &error_code ) {

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


	void	ret_request_http( const std::map<std::string, std::string> &request, \
		const std::string &msg, const int error_code ) {

		try {
			signal(SIGPIPE, Error_exception::socket_close);
			if (send(_socketClient, msg.c_str(), msg.size(), 0) < 0)
				Error_exception::bad_send;
			signal(SIGPIPE, SIG_DFL);
			std::ofstream	log_file;
			get_log_file(log_file, error_code);
			if (log_file.is_open()) {
				if (request.count("Host:"))
					log_file << request.at("Host:");
				log_file << " - -"<< getDateAndTime() << ' ';
				if (request.count("Methode-http:")) {
					log_file << request.at("Methode-http:") + ' ';
					if (request.count(request.at("Methode-http:")))
						log_file << request.at(request.at("Methode-http:")) \
							<< ' ' << request.at(request.at("Methode-http:")) << "\" ";
				}
				else
					log_file << " \" ";
				log_file << error_code << " " << msg.size() << " \"-\" \"";
				if (request.count("User-Agent:"))
					log_file << request.at("User-Agent:");
				log_file << "\"\n";
				log_file.close();
			}
		} catch ( const Error_exception &e ) {
			std::cerr << e.what().first << std::cout;
		}
		return ;
	}


	std::string urldecode(const std::string& str) {
		std::string result;
		char ch;
		int i, hexCode;
		for(i = 0; i < str.length(); i++) {
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
		return result;
	}

	# include "./request_http/request_get.hpp"

	# include "./request_http/request_post.hpp"

	# include "./request_http/request_delete.hpp"

};


#endif

	// 200 OK: The server has successfully processed the request.
    // 201 Created: The server has successfully created a new resource as a result of the request.
    // 204 No Content: The server has successfully processed the request, but there is no content to send back.
    // 400 Bad Request: The server cannot process the request because it is malformed or incorrect.
    // 401 Unauthorized: The client is not authorized to access the requested resource.
    // 403 Forbidden: The client does not have permission to access the requested resource.
    // 404 Not Found: The requested resource does not exist on the server.
    // 405 Method Not Allowed: The server does not support the requested method (e.g. POST, GET, PUT, DELETE) for the requested resource.
    // 500 Internal Server Error: The server encountered an unexpected condition that prevented it from fulfilling the request.


		// std::cout << request["Request_content"].size() << std::endl;
		
		// std::string	boundary = request[ \
		// 		"Content-Type"].substr(request[ \
		// 		"Content-Type"].find("boundary=") \
		// 		+ strlen("boundary="));

		// // std::cout << request["Request_content"] << std::endl;

		// std::string	tmp = request["Request_content"];
		// int pos = tmp.find(boundary);
		// tmp = tmp.substr(pos + boundary.size());
		// tmp = tmp.substr(0, tmp.size() - boundary.size() - 8);

		
		// std::string	tmp2 = tmp.substr(0, tmp.find("\r\r") + 1);
		// for (std::string::iterator it = tmp2.begin();
		// 	it != tmp2.end(); ++it)
		// 	if (*it == 13)
		// 		std::cout<<std::endl;
		// 	else
		// 		std::cout << *it << std::flush;
		// std::cout<<std::endl;


		// tmp = tmp.substr(tmp.find("PNG") - 1);

		// int	i = 0;
		// std::cout << "-----------------" << std::endl;
		// std::cout << "[";
		// for (std::string::iterator it = tmp.begin();
		// 	it != tmp.end(); ++it) {
		// 	++i;
		// 	if ((int)*it == 13)
		// 		std::cout << std::endl;
		// 	else if ((int)*it > 15)
		// 		std::cout << *it << std::flush;
		// 	else
		// 		std::cout << "(" << (int)*it << ")" << std::flush;
		// 	img.write(&*it, 1);
		// }
		// std::cout << "]" << std::endl;
		// std::cout << "-----------------" << std::endl;
		// std::cout << "size:" << i << std::endl;
		// std::cout << "size tmp:" << tmp.size() << std::endl;
		// std::cout << std::endl << std::endl;

		//  std::ifstream is ("./template.png", std::ifstream::binary);
		//   if (is) {
		//     // get length of file:
		//     is.seekg (0, is.end);
		//     int length = is.tellg();
		//     is.seekg (0, is.beg);

		//     char * buffer = new char [length];

		//     std::cout << "Reading " << length << " characters... ";
		//     // read data as a block:
		//     is.read (buffer,length);

		//     if (is)
		//       std::cout << "all characters read successfully." << std::endl;
		//     else
		//       std::cout << "error: only " << is.gcount() << " could be read";
		//     is.close();
		//     std::cout << "[";
		//     for (int i = 0; i < length; ++i) {

		// 		if ((int)buffer[i] == 13)
		// 			std::cout << std::endl;
		// 		else if ((int)buffer[i] > 15)
		// 			std::cout << buffer[i] << std::flush;
		// 		else
		// 			std::cout << "(" << (int)buffer[i] << ")" << std::flush;
		// 	}
		// 	std::cout << "]" << std::endl;
		//     // ...buffer contains the entire file...

		//     delete[] buffer;
		// }
		// else
		// 	perror("");
