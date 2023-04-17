# include "../headers/main.hpp"


int main(int argc, char **argv)
{
	Parser	parser(argc, argv);

	std::cout << parser << std::endl;

	std::cout << "SOMAXCONN: " << SOMAXCONN << std::endl;
	std::cout << "EV_ADD: " << EV_ADD << std::endl;
	std::cout << "EV_ENABLE: " << EV_ENABLE << std::endl;
	std::cout << "EV_DISABLE: " << EV_DISABLE << std::endl;
	std::cout << "EV_DISPATCH: " << EV_DISPATCH << std::endl;
	std::cout << "EV_DELETE: " << EV_DELETE << std::endl;
	std::cout << "EV_RECEIPT: " << EV_RECEIPT << std::endl;
	std::cout << "EV_ONESHOT: " << EV_ONESHOT << std::endl;
	std::cout << "EV_CLEAR: " << EV_CLEAR << std::endl;
	std::cout << "EV_EOF: " << EV_EOF << std::endl;
	std::cout << "EV_ERROR: " << EV_ERROR << std::endl;
	std::cout << "EV_ADD | EV_ONESHOT: " << (EV_ADD | EV_ONESHOT) << std::endl;

	Server	serv(parser[0]);

	serv.start();

	if (LEAKS)
		system("leaks webserver");

	return (0);
}
