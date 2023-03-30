// #ifndef __CLIENT_HPP__
// # define __CLIENT_HPP__


// class Client
// {
// private:
// 	pthread_t		_thread;
// 	int				_socketServer;
// 	int				_socketClient;

// 	Client( void );

// public:
// 	Client(int socketServer) : _socketServer(socketServer) {

// 		std::cout << "Constructor Client called" << std::endl;
// 	}

// 	~Client() {
		
// 		close (_socketClient);

// 		std::cout << "Destructor Client called" << std::endl;
// 	}

// 	void	run( void ) {
// 		routine();
// 	}

// private:
// 	void	*routine( void ) {

// 		if ((_socketClient = accept(_socketServer, NULL, NULL)) < 0) {
// 			perror("ERRPR accept"); exit(EXIT_FAILURE);
// 		}

// 		char	msg[4096];

// 		if (recv(_socketClient, msg, sizeof(msg), 0) < 0) {
// 			perror("ERROR recv"); exit(EXIT_FAILURE);
// 		}
// 		std::cout << "Message: [" << msg << " ]" << std::endl;
// 		close (_socketClient);
// 		return (NULL);
// 	}

// };


// #endif
