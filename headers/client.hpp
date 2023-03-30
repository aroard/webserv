#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__


class Client
{
private:
	// std::thread		thread;
	int				_socketServer;
	int				_socketClient;

	Client( void );

public:
	Client(int socketServer) : _socketServer(socketServer) {

		if ((_socketClient = accept(_socketServer, NULL, NULL)) < 0) {
			perror("ERRPR accept"); exit(EXIT_FAILURE);
		}

		std::cout << "Constructor Client called" << std::endl;
	}

	~Client() {
		
		close (_socketClient);

		std::cout << "Destructor Client called" << std::endl;
	}

	void	routine( void ) {

		char	msg[4096];

		while (true) {
			if (recv(_socketClient, msg, sizeof(msg), 0) < 0) {
				perror("ERROR recv"); exit(EXIT_FAILURE);
			}
			std::cout << "Message: [" << msg << " ]" << std::endl;
		}
		return ;
	}

};


#endif
