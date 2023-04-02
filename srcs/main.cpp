# include "../headers/main.hpp"


int main(int argc, char **argv)
{
	Parser	parser(argc, argv);
	Server	serv("127.0.0.1", parser[0]);

	pthread_t	thread[1];
	thread[0] = serv.start();
	pthread_join(thread[0], NULL);

	return (0);
}
