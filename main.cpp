#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

#define PORT 8080

int main()
{
	int server_fd, new_socket;
	long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	std::string hello = "Hello from server";
	std::cout << "test" << std::endl;
	if ((server_fd  = socket(AF_INET, SOCK_STREAM, 0)) == 0) // AF_INET = address_family SOCK_STREAM virtual circuit service; ici par default pour faire une communication TCP/IP
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT);

	memset(address.sin_zero, '\0', sizeof address.sin_zero);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	while(42)
	{
		std::cout << "\n+++++++ Waiting for new connection ++++++++" << std::endl;
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}
		char buffer[30000] = {0};
		valread = read( new_socket, buffer, 30000);
		std::cout << buffer << std::endl;
		write(new_socket, hello.c_str(), strlen(hello.c_str()));
		std::cout << "------------------Hello message sent-------------------" << std::endl;
		close(new_socket);
	}
	return (0);
}
