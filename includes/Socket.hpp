
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Client.hpp"
#include "webserv.hpp"

class Socket {
	public:
		Socket();
        ~Socket();

		void init_socket( void);
		void ft_bind( void);
		void ft_listen( void);
		void init_pfd( void );
		void ft_poll( void );
		int check_pfd( void );
		void server( void );
		int ft_accept( int i);
		int ft_clean_up( void );
		int ft_close_pfd( void );
		Parsing *get_parsing( void);
		void set_parsing(Parsing *parsing);
		int check_dead_pfd(int i);
		bool	is_a_socket_master(int socket);
	

	private:
		struct sockaddr_in addr[2];
		struct pollfd pfd[32];
		char buffer_in[1024];
		char buffer_out[1024];
		int socket_fd[32];
		int nbr_server;
		int new_fd;
		int clean_up;
		int end_serv;
		bool purge;
		int current_size;
		int nfds;
		int timeout;
		int ret;
		int rc;
		// int len;
		// int end_co;
		std::vector<std::string> list_port;
		std::map<int , Client *> client;
		Request request;
		Parsing	*parsing;
		
};

#endif
