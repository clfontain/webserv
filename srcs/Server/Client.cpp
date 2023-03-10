#include "Client.hpp"

Client::Client()
{
	this->end_co = false;
	this->All_recv = false;
	this->Content_length = false;
	this->Transfer_encoding = false;
	this->header_recv = false;
	this->socket = -1;
}

Client::Client(int socket)
{
	this->end_co = false;
	this->socket = socket;
}

Client::~Client()
{

}

int Client::find_header()
{
	size_t found = 0;
	found = this->recv_str.find("\r\n\r\n", 0);
	for(size_t i = 0; i < strlen(this->recv_str.c_str()); i++)
	{
		//std::cout << (int)this->recv_str[i] << std::endl;
	}
	
	if (found == std::string::npos)
	{
		std::cout << "Not all header yet" << std::endl;
	}
	else
		this->header_recv = true;
	return 0;
}

void Client::parse_content_length(std::string str)
{
	size_t i = 0;
	size_t j = 0;
	while (i < str.length() && isspace(str[i]))
		i++;
	if (i == str.length())
	{
		std::cout << "Content Length ERROR\n";
		return;
	}	
	while (i < str.length() && isdigit(str[i]))
		i++;
	j = i;
	while (j < str.length() && isspace(str[j]))
		j++;
	if (j > 9)
	{
		std::cout << "Content Length ERROR\n TOO BIG\n";
		return;
	}
	i = j;
	if (i != str.length() || str.length() == 0)
	{
		std::cout << "Content Length ERROR\n";	
		return;
	}	
	else
		this->length_expected = atoi (str.c_str());
}

void Client::check_lenght_body()
{
	this->request_line = cpsplit(this->recv_str, "\r\n");
	size_t found = 0;
	for (size_t i = 0; i < this->request_line.size(); i++)
	{
		found = this->request_line[i].find("Content-Length:", 0);
		if (found != std::string::npos)
		{
			if (Content_length == true)
			{
				std::cout << "Error\n";
				return;
			}	
			Content_length = true;
			parse_content_length(this->request_line[i].c_str() + 15);
		}	
		found = this->request_line[i].find("Transfer-Encoding:", 0);
		if (found != std::string::npos)
		{
			Transfer_encoding = true;
			//parse_transfert_encoding();
		}	
	}
}

int	Client::ft_recv( int i )
{
	int rc;
	(void)i;
	memset(this->buffer_in, 0, sizeof(this->buffer_in));
	std::cout << YELLOW "rec with i : " << i << "pfd = " << socket << std::endl;
	rc = recv(socket, buffer_in, sizeof(buffer_in), 0);
	if (rc < 0)
	{
		perror("recv() failed");
			this->end_co = true;
		return (false);
	}
	if (rc == 0)
	{
		std::cout << "Connection closed" << std::endl;
		this->end_co = true;
		return (false);
	}
	this->timer = std::time(NULL);
	(void)len_recv;
	this->recv_str += buffer_in;
	this->find_header();
	if (this->header_recv == true)
		check_lenght_body();
	//this->recv_str.append(this->buffer_in);
	//this->len_recv += rc;
	//std::cout << ".......MESSAGE DU CLIENT.......\n" << buffer_in;
	/*std::cout << rc << " bytes received" << std::endl;
	std::cout << this->len_recv << "TOTAL bytes received" << RESET << std::endl;
	std::cout << this->recv_str << std::endl;*/
	//if (this->All_recv == true)
	//{
	this->request.SetParsing(this->parsing);
	this->request.SetRequest(buffer_in);
	this->request.Process_request();
	this->request.GetResponse();
	//}	
	// std::cout << this->request.GetResponse() << std::endl;
	return (true);
}

int Client::ft_send( int i )
{
	int rc;
	(void)i;
	memset(this->buffer_out, 0, sizeof(this->buffer_out));
	//const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	//std::cout << "TOUT FONCTIONNE BIEN" << std::endl;
	//std::cout << strlen(this->send_str.c_str()) << std::endl;
	rc = send(socket, this->request.GetResponse().c_str(), strlen(this->request.GetResponse().c_str()), 0);
	//rc = send(socket, hello, strlen(hello), 0);
	if (rc < 0)
	{
		perror("send() failed");
		this->end_co = true;
		return (false);
	}
	//// IMPORTANT IL FAUT CHECKER AUSSI 0 SINON C'EST ZERO !!!!!!!!!!!!!!!
	this->len_send += rc;
	return (false);
}

int	Client::fd_ready_IO( int i)
{
	//std::cout << "DESCRIPTOR IS READABLE" << pfd[i].fd << std::endl;
	this->end_co = false;
	ft_recv(i);
	//if (this->All_recv == true)
		ft_send(i);
	return (0);
}

bool Client::Get_end_co( void) const
{
	return (this->end_co);
}

void Client::set_parsing(Parsing *parsing)
{
	this->parsing = parsing;
}


bool Client::Get_header_recv()
{
	return (this->header_recv);
}

bool Client::Get_All_recv()
{
	return (this->All_recv);
}

time_t Client::Get_timer()
{
	return (this->timer);
}

void Client::Set_end_co(bool end_co)
{
	this->end_co = end_co;
}

void Client::Set_timer(time_t timer)
{
	this->timer = timer;
}
