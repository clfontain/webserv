/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mede-sou <mede-sou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 11:20:13 by mede-sou          #+#    #+#             */
/*   Updated: 2023/02/24 17:05:03 by mede-sou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/****************************** INITIALISATION ********************************
******************************************************************************/

Server::Server() : autoindex(false), sublocation(false) { }

Server::Server(Server const &cpy) : methods(cpy.getMethods()), name(cpy.getName()),
	port(cpy.getPort()), host(cpy.getHost()), index(cpy.getIndex()),
		error_code(cpy.getErrorCode()), error_page(cpy.getErrorPage()),
			body_size(cpy.getBodySize()), root(cpy.getRoot()), cgi_pass(cpy.getCgiPass()) {
				*this = cpy;
}

Server & Server::operator=(Server const &assign) { 
	(void)assign;
	return *this; 
}

Server::~Server() {
	for (std::size_t i = 0; i < this->vec_location.size(); i++)
		delete this->vec_location[i];
 }

/********************************* UTILS **************************************
******************************************************************************/

std::string Server::cutwhitespace(std::string str)
{
	int j = str.length() - 1;
	int i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	while (str[j] && (str[j] == ' ' || str[j] == '\t'))
		j--;
	std::string new_str = str.substr(i, j - i + 1);
	return new_str;
}

std::string Server::firstword(std::string str, int i)
{
	int j = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (i != 0)
    	j = i;
	while (str[j] && (str[j] != ' ' && str[j] != '\t'))
		j++;
	if (str[j] == ' ' || str[j] == '\t')
		j--;
	std::string new_str = str.substr(i, j - i + 1);
	return new_str;
}

std::string Server::checksemicolon(std::string content) {
	std::size_t len = content.length() - 1;
	while (content[len] == ' ' || content[len] == '\t')
		len--;
	if (content[len] != ';')
	{
		std::cerr << "Error, semicolon missing for instruction\n";
		exit(EXIT_FAILURE);
	}
	content.erase(len, 1);
	return (content);
}

std::vector<std::string> Server::cppsplit(std::string str, char sep)
{
	int i = 0;
	int j = 0;
	std::vector<std::string> vec;
	std::string tmp;
	while (str[i] != 0)
	{
		while (str[i] != 0 && str[i] == sep)
			i++;
		j = i;
		while (str[j] != 0 && str[j] != sep)
			j++;
		if (j != i)
		{
			tmp = str.substr(i, (j - i));
			vec.push_back(tmp);
		}	
		i = j;
	}	
	return (vec);
}

std::string Server::extractcontent(std::vector<std::string>::iterator it, int i, std::string keyword)
{
	int j = 0;
	while ((*it)[j] == ' ' || (*it)[j] == '\t')
		j++;
	std::size_t found = (*it).find('#');
	std::string content;
	if (found != std::string::npos)
		content = (*it).substr(keyword.length() + j, 
			((*it).length() - keyword.length()) - ((*it).length() - found + 1));
	else
		content = (*it).substr(keyword.length() + j, (*it).length());
	if (i != 9)
		content = checksemicolon(content);
	return (content);
}

void Server::check_doubl_kw(std::vector<std::string> vec_kw)
{
	std::vector<std::string>::iterator it = vec_kw.begin();
	for (it = vec_kw.begin(); it != vec_kw.end(); ++it)
	{
		if (std::count(vec_kw.begin(), vec_kw.end(), *it) > 1 && (*it) != "}" && (*it) != "error_page" && (*it) != "location")
		{
			std::cerr << "Error: same field cannot be configured more than once\n";
			exit(EXIT_FAILURE);
		}
	}
}
/********************************* GETERS *************************************
******************************************************************************/

std::string Server::getMethods() const { return (this->methods); }
std::string Server::getName() const { return (this->name); }
std::string Server::getPort() const { return (this->port); }
std::string Server::getHost() const { return (this->host); }
std::string Server::getIndex() const { return (this->index); }
int Server::getErrorCode() const { return (this->error_code); }
std::string Server::getErrorPage() const { return (this->error_page); }
int Server::getBodySize() const { return (this->body_size); }
std::string Server::getRoot() const { return (this->root); }
bool Server::getAutoIndex() const { return (this->autoindex); }
std::string Server::getPathAutoIndex() const { return (this->pathAutoIndex); }
std::string Server::getCgiPass() const { return (this->cgi_pass); }
std::vector<Location *> Server::getVectorLocation() const { return (this->vec_location); }

/********************************* SETERS *************************************
******************************************************************************/

void Server::setMethods(std::string methods) { this->methods = methods; }
void Server::setName(std::string name) { this->name = name; }
void Server::setPort(std::string port) { this->port = port; }
void Server::setHost(std::string host) { this->host = host; }
void Server::setIndex(std::string index) {
	this->index = this->root + "/" + index;
}

void Server::setErrorCode(int error_code) { this->error_code = error_code; }
void Server::setErrorPage(std::string error_page) { this->error_page = error_page; }
void Server::setBodySize(int body_size) { this->body_size = body_size; }
void Server::setRoot(std::string root) { this->root = root; }
void Server::setCgiPass(std::string cgi_pass) { this->cgi_pass = cgi_pass; }

void Server::setAutoIndex(std::string autoindex) {
	std::string pathAutoIndex;
	if (autoindex == "on")
	{
		this->autoindex = true;
		pathAutoIndex = this->root;
	}
	else
		pathAutoIndex = "";
	this->pathAutoIndex = pathAutoIndex;
}

/******************************* CHECK ERRORS *********************************
******************************************************************************/

void Server::check_server_name(std::string content) { setName(content);} 
// utile ? a quoi sert le name

void Server::check_listen(std::string content) { 	
	
	std::vector<std::string> listen = cppsplit(content, ':');
	if (listen.size() == 2)
	{
		std::vector<std::string>::iterator it = listen.begin();
		std::string host = *it;
		setHost(host);
		it++;
		std::string port = *it;
		setPort(port);
	}
	else 
	{
		int count_p = 0;
		bool is_digit = true;
		for (int j = 0; content[j]; j++)
		{
			if (content[j] == '.')
				count_p++;
			else if (!isdigit(content[j]))
				is_digit = false;
		}
		if (count_p == 3)
			setHost(content);
		else if (is_digit == true)
			setPort(content);
	}
}

void Server::check_root(std::string content) {
	std::vector<std::string> root = cppsplit(content, ' ');
	if (root.size() > 1)
	{
		std::cerr << "Error, root has to be a single file\n";
		exit(EXIT_FAILURE);
	}
	setRoot(content);
}

void Server::check_index(std::string content) {
	std::vector<std::string> index = cppsplit(content, ' ');
	if (index.size() > 1)
	{
		std::cerr << "Error, index has to be a single file\n";
		exit(EXIT_FAILURE);
	}
	setIndex(content);
}

void Server::check_auto_index(std::string content) {
	if (content.compare("on") != 0 && content.compare("off") != 0)
	{			
		std::cerr << "Error, autoindex has to be either on or off\n";
		exit(EXIT_FAILURE);
	}
	setAutoIndex(content);
}

void Server::check_error_page(std::string content) {
	std::vector<std::string> vec_error_page = cppsplit(content, ' ');

	if (vec_error_page.size() > 2)
	{
		std::cerr << "Error, error_page has to be a single file\n";
		exit(EXIT_FAILURE);
	}
	std::string error_code = vec_error_page[0];
	if (atoi(error_code.c_str()) < 400 || atoi(error_code.c_str()) > 599)
	{
		std::cerr << "Error, error code has to be between 400 et 599\n";
		exit(EXIT_FAILURE);
	}
	std::string file = vec_error_page[1];
	// if (file.compare("404 html") != 0)
	// {
	// 	std::cerr << "Error, this error page doesn't exist\n";
	// 	exit(EXIT_FAILURE);
	// }
	setErrorPage(file);
	setErrorCode(atoi(error_code.c_str()));
}

void Server::check_allow_methods(std::string content) {
	std::vector<std::string> methods = cppsplit(content, ' ');
	std::string new_content;
	
	if (methods.size() > 3)
	{			
		std::cerr << "Error, method is not allowed\n";
		exit(EXIT_FAILURE);
	}

	std::vector<std::string>::iterator it = methods.begin();
	for (; it != methods.end() ; ++it)
	{
		if ((*it).compare("GET") == 0 || (*it).compare("POST") == 0 || (*it).compare("DELETE") == 0)
			new_content += (*it) + ' '; 
		else
		{			
			std::cerr << "Error, method is not allowed\n";
			exit(EXIT_FAILURE);
		}
	}
	setMethods(new_content);
}

void Server::check_client_max_body_size(std::string content) {
	std::string cut = firstword(content, 0);
	int size = 1000000;
	for (unsigned long int i = 0; i < content.length() - 2; i++) {
		if (!isdigit(cut[i]))
		{			
			std::cerr << "Error, wrong body size format\n";
			exit(EXIT_FAILURE);
		}
	}
	if (content[content.length() - 1] != 'm' && content[content.length() - 1] != 'M')
	{			
		if (!isdigit(content[content.length() - 1]))
		{
			std::cerr << "Error, wrong body size format\n";
			exit(EXIT_FAILURE);
		}
		size = atoi(cut.c_str());
	}
	else if (content[content.length() - 1] == 'm' || content[content.length() - 1] == 'M')
	{
		size = atoi(cut.c_str());
		size *= 1000000;
	}
	if (size > 50000000)
	{
		std::cerr << "Error, your file must be less than 50MB\n";
		exit(EXIT_FAILURE);
	}
	setBodySize(size);
}

void Server::check_cgi_pass(std::string content) {
	if (content.compare("./srcs/CGI/scripts/php-cgi") != 0)
	{
		std::cerr << "Error, cgi_pass not valid\n";
		exit(EXIT_FAILURE);
	}
	setCgiPass(content);
}

void Server::check_each_element(std::string content, int i)
{
	content = cutwhitespace(content);
	switch (i) 
	{
		case 0 : check_server_name(content);
            break;
		case 1 : check_listen(content);
			break;
		case 2 : check_root(content);
			break;
		case 3 : check_index(content);
			break;
		case 4 : check_allow_methods(content);
			break;
		case 5 : check_error_page(content);
			break;
		case 6 : check_client_max_body_size(content);
			break;
		case 7 : check_auto_index(content);
            break;
		case 8 : check_cgi_pass(content);
			break;
	}
}


/**************************** CREATE LOCATION *********************************
******************************************************************************/

void Server::copy_server_to_location(Location &location) {
	if (vec_location.size() == 0 || sublocation == false)
	{
		location.setHost(this->getHost());
		location.setPort(this->getPort());
		location.setMethods(this->getMethods());
		location.setRoot(this->getRoot());
		// location.setIndex(this->getIndex());
		location.setAutoIndex("off");
		location.setErrorPage(this->getErrorPage());
		location.setErrorCode(this->getErrorCode());
		location.setBodySize(this->getBodySize());
	}
	else
	{
		std::vector<Location *>::iterator ite = vec_location.end();
		--ite;
		location.setHost((*ite)->getHost());
		location.setPort((*ite)->getPort());
		location.setMethods((*ite)->getMethods());
		location.setRoot((*ite)->getRoot());
		// location.setIndex((*ite)->getIndex());
		location.setAutoIndex("off");
		location.setErrorPage((*ite)->getErrorPage());
		location.setErrorCode((*ite)->getErrorCode());
		location.setBodySize((*ite)->getBodySize());
	}
}

std::vector<std::string>::iterator Server::createNewLocation(std::vector<std::string>::iterator it) {
	const std::string tab[8] = {"listen", "root", "index", "allow_methods",
		"error_page", "client_max_body_size", "autoindex", "cgi_pass"};
	Location* location = new Location();

	std::vector<std::string>::iterator tmp_it  = it;
	std::vector<std::string> vec_kw;

	copy_server_to_location(*location);
	location->check_each_element(extractcontent(it, 9, "location"), 9);

	for (; it != vec_file.end(); ++it)
	{
		std::string keyword = firstword(*it, 0);

		if (keyword.compare("location") == 0 && tmp_it != it)
		{
			sublocation = true;
			break;
		}
		for (int i = 0; i < 8; i++)
		{
			if (!keyword.empty() && keyword.compare(tab[i]) == 0)
			{
				location->check_each_element(extractcontent(it, i, keyword), i);
				vec_kw.push_back(keyword);
				break ;
			}
			else if (i == 7 && !keyword.empty() && keyword.compare("location") != 0 
				&& keyword.compare("server") != 0 && keyword.compare("}") != 0
					&& keyword.compare("#") != 0)
			{
				std::cerr << "Error file: wrong keyword\n";
				exit(EXIT_FAILURE);
			}
		}
		if (keyword.compare("}") == 0)
		{
			sublocation = false;
			break;
		}
	}
	if (location->getIndex().empty())
		location->setIndex(this->getIndex());
	check_doubl_kw(vec_kw);
	vec_location.push_back(location);
	count_location++;
	if (sublocation == true)
		it--;
	return (it);

}

/******************************** PRINT ***************************************
******************************************************************************/

void Server::printServer()
{
	std::cout << "name: " << name << "\n";
	std::cout << "host: " << host << "\n";
	std::cout << "port: " << port << "\n";
	std::cout << "methods: " << methods << "\n";
	std::cout << "root: " << root << "\n";
	std::cout << "index: " << index << "\n";
	std::cout << "autoindex: " << autoindex << "\n";
	std::cout << "path autoindex: " << pathAutoIndex << "\n";
	std::cout << "error_page: " << error_page << "\n";
	std::cout << "cgi_pass: " << cgi_pass << "\n";
	std::cout << "body_size: " << body_size << "\n\n";
}

void Server::printLocationVector() {
	std::vector<Location *>::iterator it;
	std::cout << "CE SERVEUR POSSEDE " << vec_location.size() << " LOCATION\n";
	
		for (it = vec_location.begin(); it != vec_location.end(); ++it)
		{
			(*it)->printLocation();
		}
}
