#include "webserv.hpp"



void print_map(std::map<std::string, std::string> mymap)
{
	std::map<std::string, std::string>::iterator m_it;
    for (m_it = mymap.begin(); m_it != mymap.end(); ++m_it)
    {
		std::cout << m_it -> first << "\n";
		std::cout << m_it -> second << "\n";
    }
}

std::string split_space(std::string str, int *npos)
{
	std::string new_str;
	int i;
	int j = (*npos);
	for(i = (*npos); str[i] != ' ' && str[i] != '\n' && str[i] != 0; i++) {
	}
	new_str = str.substr(j, (i - j));
	while (str[i] == ' ')
		i++;
	(*npos) = i;
	return (new_str);
}

std::string read_file(std::string file_name)
{
	std::string buff;
	std::string final;
	(void)file_name;
	std::ifstream infile(file_name.c_str() + 1);
	//std::ifstream infile("index.html");
	if (infile.is_open() == false)
	{
		std::cerr << "Error infile is missing" << std::endl;
		return (NULL);
	}
	while (infile.good())
	{
		std::getline(infile, buff);
		final.append(buff);
		if (infile.eof() == true)
			break;
		final.append("\n");
	}
	infile.close();
	std::cout << "len = " << strlen(final.c_str()) << std::endl; 
	char * len = itoa(strlen(final.c_str()));
	std::cout << len<< std::endl;
	//std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ";
	//response += len;
	//response += "\n\n";
	//response = response + final;
	std::string response = final;
	std::cout << response;
	return (response);
}



// void parsing_client(s_server *server, char *buffer)
// {
// 	server->request = buffer;
// 	server->v_req = cpsplit(server->request, ' ');
// 	server->v_req_path = cpsplit(server->v_req[1], '/');
// 	//print_vector(server->v_req_path);
// 	server->req.SetReq_location(server->v_req_path);
// 	request(server);
// }



int	server(int port, Parsing *parsing)
{
	(void)port;
	Socket server;
	server.set_parsing(parsing);
	server.server();
	return (0);
}
