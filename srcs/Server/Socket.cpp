#include "Socket.hpp"

Socket::Socket()
{
	this->clean_up = false;
	this->end_serv = false;
	this->purge = false;
	this->nbr_server = 0;
	this->new_fd = -1;
	this->nfds = 1;
	this->timeout = (60 * 1000);
	memset(this->buffer_in, 0, sizeof(this->buffer_in));
	memset(this->buffer_out, 0, sizeof(this->buffer_out));
}

Socket::~Socket()
{

}




/****************************** INIT_SOCKET **********************************
socket : Créer un point de communication et renvoie un descripteur
setsockopt : Permet de rendre le socket non bloquant
fcntl : Permet la réutilisation des sockets liés à ce port, evite le TIME_WAIT
******************************************************************************/

void Socket::init_socket( void)
{
	this->nbr_server = 1; // POUR LE TEST ON MET DEUX SERVEURS
	for (int i = 0; i < this->nbr_server; i++)
	{
		int on = 1;	
		this->socket_fd[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (this->socket_fd[i] == -1)
		{
			perror("socket() failed"); //virer les perror
			exit (EXIT_FAILURE);
		}
		this->rc = setsockopt(this->socket_fd[i], SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		 if (this->rc < 0)
		{
			perror("setsockopt() failed");
			close(this->socket_fd[i]);
			exit (EXIT_FAILURE);
		}
		this->rc = fcntl(this->socket_fd[i], F_SETFL, O_NONBLOCK);
		if (this->rc < 0)
		{
			perror("fcntl() failed");
			close(this->socket_fd[i]);
			exit (EXIT_FAILURE);
		}
	}
}

/********************************** FT_BIND ***********************************
Personne ne sait l'intérêt de sin_zero, les gens mettent la variable à zero 
alors on va faire pareil.
******************************************************************************/

void    Socket::ft_bind( void )
{
	int port;
	list_port.push_back("8000");
	//list_port.push_back("8081");
	for (int i = 0; i < nbr_server; i++)
	{
		this->addr[i].sin_family = AF_INET;
		this->addr[i].sin_addr.s_addr = INADDR_ANY; /* use my IP adresse */
		port = atoi(this->list_port[i].c_str());
		this->addr[i].sin_port = htons(port);
		memset(this->addr[i].sin_zero, 0, sizeof(this->addr[i].sin_zero));
		if (bind(socket_fd[i], (struct sockaddr*)&addr[i], sizeof(sockaddr))	< 0)
		{
			std::cout << "bind() failed" << strerror(errno) << std::endl;
			close(socket_fd[i]);
			exit(EXIT_FAILURE);
		}
	}    
}

/********************************** FT_LISTEN**********************************
******************************************************************************/

void    Socket::ft_listen( void )
{
	for (int i = 0; i < nbr_server; i++)
	{
		if (listen(socket_fd[i], 1000) < 0)
		{
			perror("listen() failed");
			close(socket_fd[i]);
			exit(EXIT_FAILURE);
		}
	}        
}

/********************************** INIT_PFD***********************************
******************************************************************************/

void Socket::init_pfd( void )
{
	memset(pfd, 0, sizeof(pfd));
	for (int i = 0; i < nbr_server; i++)
	{
		this->pfd[i].fd = this->socket_fd[i];
		this->pfd[i].events = POLLIN;
	}
	  
}

void Socket::ft_poll( void )
{
	do
	{
		std::cout << "IN POLL LOOP nfds = "<< nfds << std::endl;
		ret = poll(pfd, nfds, timeout);
		for (int i = 0; i < nfds; i++)
		{
			//std::cout << "pfd.fd = "<< pfd[i].fd << "pfd.events =" << pfd[i].events << "pfd.revents =" << pfd[i].revents << std::endl;
		}
		//std::cout << "AFTER POLL" << std::endl;
		if (ret < 0)
		{
			perror("poll() failed");
			break;
		}
		current_size = nfds;
		if (ret == 0)
		{
			for (int i = 0; i < current_size; i++)
			{
				if (this->is_a_socket_master(pfd[i].fd) == 0)
				{
					if (check_dead_pfd(i) == EXIT_FAILURE)
						purge = true;
				}		
			}
			if (purge == false)
				continue;
		}
		check_pfd();
		if (clean_up == TRUE)
			ft_clean_up();
	} while (end_serv == FALSE);
}

int Socket::check_pfd( void )
{
	for (int i = 0; i < current_size; i++)
	{
			if (pfd[i].revents == 0)
			{
				if (this->is_a_socket_master(pfd[i].fd) == 0)
				{
					if (check_dead_pfd(i) == EXIT_SUCCESS)
						continue;
				}		
			}
			else if (pfd[i].revents != POLLIN)
			{
				std::cout << "ERROR ! revents = " << pfd->revents << std::endl;
				end_serv = TRUE;
				break;
			}
			if (this->is_a_socket_master(pfd[i].fd) == 1)
			{
				do
				{
					if (ft_accept(i) == FALSE)
						break;
				} while (this->new_fd != -1);
			}
			else
			{
				std::map<int , Client *>::iterator it;
				it = this->client.find(pfd[i].fd);
				if	(it->second->Get_end_co() == false)
				{
					it->second->set_parsing(this->get_parsing());
					it->second->fd_ready_IO(i);
				}
				if (it->second->Get_end_co() == true)
				{
					this->client.erase(pfd[i].fd);
					close(pfd[i].fd);
					pfd[i].fd = -1;
					clean_up = true;
				}
			}
	}
	return (0);
}

bool	Socket::is_a_socket_master(int socket)
{
	for (int i = 0; i < nbr_server; i++)
	{
		if (socket == socket_fd[i])
			return (TRUE);
	}	
	return (FALSE);
}

int	Socket::ft_accept( int i)
{
	//std::cout << "Listening socket is readable i =" << i  << "pfd = " << pfd[i].fd << std::endl;
	new_fd = accept(socket_fd[i], NULL, NULL);
	if (new_fd < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			perror("accept() failed");
			end_serv = TRUE;
		}
		return (FALSE);
	}
	this->client.insert(std::pair<int , Client *> (new_fd, new Client(new_fd)));
	std::map<int , Client *>::iterator it;
	it = this->client.find(new_fd);
	it->second->Set_timer(std::time(NULL));
	std::cout << it->second->Get_timer() << std::endl;
	std::cout << "NEW CONNECTION " << std::endl;
	pfd[nfds].fd = new_fd;
	pfd[nfds].events = POLLIN;
	nfds++;
	return (TRUE);
}



void Socket::set_parsing(Parsing *parsing)
{
	this->parsing = parsing;
}

Parsing *Socket::get_parsing( void)
{
	return (this->parsing);
}

int Socket::ft_clean_up( void )
{
	clean_up = FALSE;
	for(int i = 0; i < nfds; i++)
	{
		if (pfd[i].fd == -1)
		{
			for(int j = i; j < nfds; j++)
			{
				pfd[j].fd = pfd[j+1].fd;
			}
			i--;
			nfds--;
		}
	}
	return (0);
}

int	Socket::ft_close_pfd( void )
{
	for (int i = 0; i < nfds; i++)
	{
		if (pfd[i].fd >= 0)
			close(pfd[i].fd);
	}
	return (0);
}

void Socket::server( void )
{
	this->init_socket();
	this->ft_bind();
	this->ft_listen();
	this->init_pfd();
	this->ft_poll();
	this->ft_close_pfd();
}

int Socket::check_dead_pfd(int i)
{
	std::map<int , Client *>::iterator it;
	it = this->client.find(pfd[i].fd);
	if (it->second->Get_end_co() == true)
		return (EXIT_FAILURE);
	time_t actual_time;
	time_t res;
	actual_time = std::time(NULL);
	res = it->second->Get_timer();
	if (res != 0)
	{
		if ((actual_time - res)  > 60)
		{
			it->second->Set_end_co(true);
			return (EXIT_FAILURE);
		}	
	}
	return (EXIT_SUCCESS);
}
