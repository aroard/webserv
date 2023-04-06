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
		// std::cout << "Constructor Client called" << std::endl;
	}

	~Client() {
		close (_socketClient);
		// std::cout << "Destructor Client called" << std::endl;
	}

	void	run( void ) {
		routine();
	}

private:

	void	routine( void ) {
		std::string	msg;
		char	*tmp = new char[_parser.get_limit_request(0) + 1];
		int 	nb_octets;

		do {
			std::memset(tmp, 0, _parser.get_limit_request(0));
			if ((nb_octets = recv(_socketClient, tmp, _parser.get_limit_request(0), 0)) < 0) {
				perror("ERROR recv"); delete[] tmp; return ;
			}
			msg.insert(msg.size(), tmp, nb_octets);
			std::cout << "x:" << nb_octets << std::endl;
		} while (nb_octets == _parser.get_limit_request(0));
		parse_request_http(msg);
		delete[] tmp;
	}

	void	parse_request_http( std::string msg ) {

		std::map<std::string, std::string> request;
		std::stringstream	ss(msg);
		std::string			line;
		size_t				pos;

		if (std::getline(ss, line)) {
			pos = line.find(' ');
			request[line.substr(0, pos)] = line.substr(pos + 1, line.size() - pos - 2);
		}
		while (std::getline(ss, line)) {
			if (line.size() == 1 && line[0] == 13) {
				std::string	tmp;
				while (std::getline(ss, line))
					tmp += line;
				request["Request_content"] = tmp;
			}
			else if (line.size() > 0) {
				pos = line.find(":");
				request[line.substr(0, pos)] = line.substr(pos + 2, line.size() - pos - 3);
			}
		}

		for (std::map<std::string, std::string>::iterator it = request.begin();
			it != request.end(); ++it) {
			if (it->first != "Request_content")
				std::cout << "Key [" << it->first << "] Value [" << it->second << "]" << std::endl;
		}

		for (int i = 0; g_array_method[i].empty() == false; ++i)
			if (request.count(g_array_method[i]))
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
		return ;
	}

	# include "./request_http/request_get.hpp"

	void	get_request_post( std::map<std::string, std::string> &request ) {
		std::cout << "POST" << std::endl;
		
		std::ofstream	img("./test.png", std::ofstream::binary);
		
		std::cout << request["Request_content"].size() << std::endl;
		
		std::string	boundary = request[ \
				"Content-Type"].substr(request[ \
				"Content-Type"].find("boundary=") \
				+ strlen("boundary="));

		// std::cout << request["Request_content"] << std::endl;

		std::string	tmp = request["Request_content"];
		int pos = tmp.find(boundary);
		tmp = tmp.substr(pos + boundary.size());
		tmp = tmp.substr(0, tmp.size() - boundary.size() - 6);

		
		std::string	tmp2 = tmp.substr(0, tmp.find("\r\r") + 1);
		for (std::string::iterator it = tmp2.begin();
			it != tmp2.end(); ++it)
			if (*it == 13)
				std::cout<<std::endl;
			else
				std::cout << *it << std::flush;
		std::cout<<std::endl;


		tmp = tmp.substr(tmp.find("PNG") - 1);

		int	i = 0;
		std::cout << "-----------------" << std::endl;
		std::cout << "[";
		for (std::string::iterator it = tmp.begin();
			it != tmp.end(); ++it) {
			++i;
			// if ((int)*it == 13)
			// 	std::cout << std::endl;
			// else if ((int)*it > 15)
			// 	std::cout << *it << std::flush;
			// else
			// 	std::cout << "(" << (int)*it << ")" << std::flush;
			img.write(&*it, 1);
		}
		std::cout << "]" << std::endl;
		std::cout << "-----------------" << std::endl;
		std::cout << "size:" << i << std::endl;
		std::cout << "size tmp:" << tmp.size() << std::endl;
		std::cout << std::endl << std::endl;

		 std::ifstream is ("./template2.png", std::ifstream::binary);
		  if (is) {
		    // get length of file:
		    is.seekg (0, is.end);
		    int length = is.tellg();
		    is.seekg (0, is.beg);

		    char * buffer = new char [length];

		    std::cout << "Reading " << length << " characters... ";
		    // read data as a block:
		    is.read (buffer,length);

		    if (is)
		      std::cout << "all characters read successfully." << std::endl;
		    else
		      std::cout << "error: only " << is.gcount() << " could be read";
		    is.close();
		    std::cout << "[";
		    // for (int i = 0; i < length; ++i) {

			// 	if ((int)buffer[i] == 13)
			// 		std::cout << std::endl;
			// 	else if ((int)buffer[i] > 15)
			// 		std::cout << buffer[i] << std::flush;
			// 	else
			// 		std::cout << "(" << (int)buffer[i] << ")" << std::flush;
			// }
			std::cout << "]" << std::endl;

			img.close();
		    // ...buffer contains the entire file...

		    delete[] buffer;
		}
		else
			perror("");
	}

};


#endif
