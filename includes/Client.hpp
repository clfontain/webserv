#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Request.hpp"
#include <sys/socket.h>
#include <ctime>


class Client
{
	public:
		Client();
		Client(int socket);
		~Client();
		void clear();
		int fd_ready_IO( int i);
		int find_header();
		int ft_recv( int i);
		int check_lenght_body();
		void Set_end_co( bool end_co);
		void Set_allowed_send( bool end_co);
		bool Get_end_co( void ) const;
		bool Get_All_send( void ) const;
		bool Get_All_recv();
		bool Get_header_recv();
		void parse_content_length(std::string str);
		int parse_transfer_encoding(std::string str);
		void parse_body_length();
		time_t Get_timer();
		void Set_timer(time_t timer);
		void set_parsing(Parsing *parsing);
		int ft_send( int i);
	private:

	bool	header_recv;
	bool	Content_length;
	bool	All_recv;
	bool	All_send;
	bool	allowed_send;
	char	buffer_in[1024];
	char	buffer_out[1024];
	std::string recv_str;
	Parsing *parsing;
	std::string send_str;
	std::vector<std::string> request_line;
	// bool all_recv;
	int len_recv;
	unsigned long int length_expected;
	int len_send;
	bool end_co;
	int socket;
	Request request;
	time_t timer;
};

std::string example_post();

#endif
