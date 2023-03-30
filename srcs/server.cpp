// # include <iostream>
// # include <sys/socket.h>
// # include <netinet/in.h>
// # include <netdb.h>
// # include <arpa/inet.h>
// # include <unistd.h>
// # include <cstring> 
// # include <pthread.h>
// # include <stdio.h>


// typedef struct User
// {
// 	char 	name[255];
// 	int		age;
// }User;


// void *function(void *arg) {

// 	int	socket;
// 	const char msg[] = "quel est votre nom et votre age ?";

// 	socket = *(int *)arg;
// 	send(socket, msg, strlen(msg), 0);

// 	User user;

// 	recv(socket, &user, sizeof(user), 0);

// 	std::cout << "user.name: " << user.name \
// 		<< std::endl << "user.age: " << user.age \
// 		<< std::endl;

// 	close(socket);
// 	delete (int *)arg;

// 	pthread_exit(NULL);
// }


// int	main(int argc, char **argv)
// {
// 	int socketServer;

// 	if ((socketServer = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
// 		perror("ERROR socket"); exit (EXIT_FAILURE);
// 	}

// 	struct sockaddr_in addrServer;

// 	addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
// 	addrServer.sin_family = AF_INET;
// 	addrServer.sin_port = htons(30000);

// 	if (bind(socketServer, \
// 		(const struct sockaddr *)&addrServer, \
// 		sizeof(addrServer)) < 0) {
// 		perror("ERROR bind"); exit (EXIT_FAILURE);
// 	}
// 	std::cout << "bind : " << socketServer << std::endl;

// 	if (listen(socketServer, 3) < 0) {
// 		perror("ERROR listen"); exit (EXIT_FAILURE);
// 	}
// 	std::cout << "listen" << std::endl;

// 	struct sockaddr_in addrClient;
// 	socklen_t csize;

// 	csize = sizeof(addrClient);

// 	pthread_t	clientThread[3];
// 	int 		socketClient;
// 	int			*socketTmp;

// 	for (int i = 0; i < 3; ++i) {
// 		if ((socketClient = accept(socketServer, \
// 			(struct sockaddr *)&addrClient, &csize)) < 0) {
// 			perror("ERROR accept"); exit (EXIT_FAILURE);
// 		}
// 		std::cout << "accept" << std::endl;
// 		std::cout << "client: " << socketClient << std::endl;
		
// 		socketTmp = new int(1);
// 		*socketTmp = socketClient;

// 		pthread_create(&clientThread[i], NULL, \
// 			function, socketTmp);
// 	}

// 	for (int i = 0; i < 3; ++i) {
// 		pthread_join(clientThread[i], NULL);
// 	}

// 	close(socketServer);
	
// 	std::cout << "close" << std::endl;

// 	return (0);
// }
// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <cstring>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>

// const int MAX_REQUEST_SIZE = 4096;
// const int MAX_RESPONSE_SIZE = 4096;

// int main()
// {
//     // Créer une socket TCP
//     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0)
//     {
//         std::cerr << "Erreur : impossible de créer une socket." << std::endl;
//         return 1;
//     }

//     // Spécifier l'adresse IP et le port du serveur
//     struct sockaddr_in serverAddress;
//     std::memset(&serverAddress, 0, sizeof(serverAddress));
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_addr.s_addr = INADDR_ANY;
//     serverAddress.sin_port = htons(8080);

//     // Lier la socket à l'adresse IP et au port du serveur
//     if (bind(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
//     {
//         std::cerr << "Erreur : impossible de lier la socket à l'adresse IP et au port." << std::endl;
//         return 1;
//     }

//     // Attendre les connexions entrantes
//     if (listen(sockfd, 5) < 0)
//     {
//         std::cerr << "Erreur : impossible d'écouter les connexions entrantes." << std::endl;
//         return 1;
//     }

//     while (true)
//     {
//         // Accepter la connexion entrante
//         struct sockaddr_in clientAddress;
//         socklen_t clientAddressLength = sizeof(clientAddress);

//         int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddress, &clientAddressLength);
//         if (clientSockfd < 0)
//         {
//             std::cerr << "Erreur : impossible d'accepter la connexion entrante." << std::endl;
//             return 1;
//         }

//         // Recevoir la requête HTTP
//         char httpRequest[MAX_REQUEST_SIZE];
//         std::memset(httpRequest, 0, sizeof(httpRequest));

//         if (recv(clientSockfd, httpRequest, sizeof(httpRequest), 0) < 0)
//         {
//             std::cerr << "Erreur : impossible de recevoir la requête HTTP." << std::endl;
//             return 1;
//         }

//         // Construire la réponse HTTP
//         std::ostringstream responseStream;

//         std::ifstream fileStream("index.html");
//         if (!fileStream.is_open())
//         {
//             std::cerr << "Erreur : impossible d'ouvrir le fichier HTML." << std::endl;
//             return 1;
//         }

//         responseStream << "HTTP/1.1 200 OK\r\n";
//         responseStream << "Content-Type: text/html\r\n";
//         responseStream << "\r\n";
//         responseStream << fileStream.rdbuf();

//         std::string response = responseStream.str();

//         // Envoyer la réponse HTTP
//         if (send(clientSockfd, response.c_str(), response.length(), 0) < 0)
//         {
//             std::cerr << "Erreur : impossible d'envoyer la réponse HTTP." << std::endl;
//             return 1;
//         }

//         // Fermer la connexion
//         close(clientSockfd);
//     }

//     // Fermer la socket
//     close(sockfd);

//     return 0;
// }
