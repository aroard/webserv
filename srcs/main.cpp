# include "../headers/main.hpp"


int main(int argc, char **argv)
{
	Parser	parser(argc, argv);

	std::cout << parser << std::endl;

	Server	serv("127.0.0.1", parser[0]);

	std::cout << (int)'\r' << " et " << (int)'\n' << std::endl;

	serv.start();

	return (0);
}
