/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfontain <cfontain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 11:01:54 by cfontain          #+#    #+#             */
/*   Updated: 2023/03/10 11:57:44 by cfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <poll.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <vector>
#include <fstream>
#include <istream>
#include <stdlib.h>
#include "Request.hpp"
#include "Parsing.hpp"
#include "Socket.hpp"
#include "Client.hpp"


#define TRUE 1
#define FALSE 0

struct s_server {
	Request req;
	Parsing pars;
	std::string request;
	std::vector<std::string> v_req;
	std::vector<std::string> v_req_path;
	std::string response;
	std::vector<std::map<std::string, std::string> > pars_v;
	std::map<std::string, std::string> req_conf;
};

char *itoa(int n);
std::vector<std::string> cpsplit(std::string test, char sep);
void	print_vector(std::vector<std::string> vec);
int request(s_server *server);
void	print_vector_m(std::vector<std::map<std::string, std::string> > vec_m);	
int	server(int port, Parsing *parsing);

#endif
