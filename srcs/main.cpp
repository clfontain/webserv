#include "Parsing.hpp"
#include "webserv.hpp"
#include "Request.hpp"
#include "cgi.hpp"
#include "colors.h"

int main(int ac, char **av)
{
	(void) av;
    Parsing parsing;

    // if (ac < 2)
    //   return (std::cerr << "Wrong number of arguments\n", 1);
    std::string file = av[1];
    parsing.init_conf_file(file);
	
	int port;
	port = 0;
	//if (ac == 2)
	port = 8080;
	if (ac == 3)
		server(port, &parsing);
	return 0;
}
