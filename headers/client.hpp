#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__


class Client
{
private:
	int				_socketClient;
	const Parser	_parser;

	Client( void );

public:
	explicit Client(int socketServer, Parser parser) : _parser(parser) {
		if ((_socketClient = accept(socketServer, NULL, NULL)) < 0) {
			perror("ERRPR accept"); exit(EXIT_FAILURE);
		}
	}

	~Client() {
		close (_socketClient);
	}

	void	run( void ) {
		routine();
	}

private:

	void	routine( void ) {
		std::map<std::string, std::string> request;
		std::string							msg;
		char								*tmp = \
				new char[_parser.get_limit_request(0) + 1];
		int 								nb_octets;
		int									pos = -1;

		std::memset(tmp, 0, _parser.get_limit_request(0));
		while ((nb_octets = recv(_socketClient, tmp, _parser.get_limit_request(0), MSG_DONTWAIT)) != 0) {
			if (nb_octets != -1) {
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
			}
			if (pos != -1 && msg.size() >= pos)
				break ;
		}
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
		choose_methode_http(request);
		delete[] tmp;
	}

// std::cout << "\033[32m████████████████ REQUEST ████████████████\033[0m" << std::endl;
// for (std::map<std::string, std::string>::iterator it = request.begin();
// 	it != request.end(); ++it) {
// 	std::cout << "["; put_line(it->first); std::cout << "]";
// 	std::cout << "("; put_line(it->second); std::cout << ")" << std::flush;
// }
// std::cout << "\033[32m█████████████████████████████████████████\033[0m" << std::endl;
// std::cout << "\033[32m████████████████ MSG ████████████████\033[0m" << std::endl;
// put_line(msg);
// std::cout << "\033[32m█████████████████████████████████████████\033[0m" << std::endl;
// std::cout << "msg:" << msg.size() << std::endl;

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


	void	choose_methode_http( std::map<std::string, std::string> &request ) {
		for (int i = 0; g_array_method[i].empty() == false; ++i) {
			if (request.count(g_array_method[i])) {
				switch (i) {
					case 0: get_request_get(request); return ;
					case 1: std::cout << "HEAD" << std::endl; return ;
					case 2: get_request_post(request); return ;
					case 3: std::cout << "OPTIONS" << std::endl; return ;
					case 4: std::cout << "CONNECT" << std::endl; return ;
					case 5: std::cout << "TRACE" << std::endl; return ;
					case 6: std::cout << "PUT" << std::endl; return ;
					case 7: std::cout << "PATCH" << std::endl; return ;
					case 8: std::cout << "DELETE" << std::endl; return ;
					default: std::cout << "Missing path" << std::endl; return ;
				}
			}
		}
		return ;
	}


	void	ret_request_http( const std::map<std::string, std::string> &request, \
		const std::string &msg, const int error_code ) {
		// std::cout << "[[[";
		// put_line(msg);
		// std::cout << "]]]" << std::endl;
		try {
			signal(SIGPIPE, Error_exception::socket_close);
			if (send(_socketClient, msg.c_str(), msg.size(), 0) < 0)
				Error_exception::bad_send;
			signal(SIGPIPE, SIG_DFL);
			std::cout << request.at("Host:")  << " - -" << getDateAndTime() \
				<< " " << request.at("Methode-http:") << " " \
				<< request.at(request.at("Methode-http:")) + "\" " \
				<< error_code << " " << msg.size() << \
				" \"-\" \"" << request.at("User-Agent:") + "\"" \
				<< std::endl;
		} catch ( const Error_exception &e) { ret_request_http(request, _parser.get_file_internal_server_error(), 500); }
		
		return ;
	}

	# include "./request_http/request_get.hpp"

	# include "./request_http/request_post.hpp"

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
