# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <cstring>

typedef struct User
{
	char 	name[255];
	int		age;
}User;


int	main(int argc, char **argv)
{
	int socketClient;

	if ((socketClient = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("ERROR socket"); exit(EXIT_FAILURE);
	}

	struct sockaddr_in addrClient;

	addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(30000);

	if (connect(socketClient, \
		(const struct sockaddr *)&addrClient, \
		sizeof(addrClient)) < 0) {
		perror("ERROR connect"); exit(EXIT_FAILURE);
	}
	std::cout << "connected" << std::endl;

	User user;
	char msg[39];

	bzero(msg, 38);
	if (recv(socketClient, msg, 38, 0) < 0) {
		perror("ERROR recv"); exit(EXIT_FAILURE);		
	}
	std::cout << msg << std::endl;
	scanf("%s %d", user.name, &user.age);

	send(socketClient, &user, sizeof(user), 0);

	close(socketClient);

	return (0);
}
