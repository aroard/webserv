# include "../headers/main.hpp"


int main()
{
	Server	serv("127.0.0.1", 8000);

	serv.start();

	return (0);
}
