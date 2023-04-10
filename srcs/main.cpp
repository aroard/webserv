# include "../headers/main.hpp"


int main(int argc, char **argv)
{
	Parser	parser(argc, argv);

	std::cout << parser << std::endl;

	Server	serv(parser[0]);

	serv.start();

	return (0);
}
