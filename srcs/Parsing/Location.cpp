/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mede-sou <mede-sou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 11:20:13 by mede-sou          #+#    #+#             */
/*   Updated: 2023/02/24 17:05:03 by mede-sou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

/****************************** INITIALISATION ********************************
******************************************************************************/
Location::Location() { }

Location::Location(Location const &cpy) : methods(cpy.getMethods()), name(cpy.getName()),
	port(cpy.getPort()), host(cpy.getHost()),
		index(cpy.getIndex()), error_page(cpy.getErrorPage()), error_code(cpy.getErrorCode()),
			body_size(cpy.getBodySize()), root(cpy.getRoot()), autoindex (cpy.getAutoIndex()), 
				pathAutoIndex (cpy.getPathAutoIndex()), cgi_pass(cpy.getCgiPass()) {
				*this = cpy;
}

Location & Location::operator=(Location const &assign) { 
	(void)assign;
	return *this; 
}

Location::~Location() { }

/********************************* GETEURS ************************************
******************************************************************************/
std::string Location::getMethods() const { return (this->methods); }
std::string Location::getName() const { return (this->name); }
std::string Location::getPort() const { return (this->port); }
std::string Location::getHost() const { return (this->host); }
std::string Location::getIndex() const { return (this->index); }
bool		Location::getAutoIndex() const { return (this->autoindex); }
std::string Location::getPathAutoIndex() const { return (this->pathAutoIndex); }
std::string Location::getErrorPage() const { return (this->error_page); }
int Location::getErrorCode() const { return (this->error_code); }
int Location::getBodySize() const { return (this->body_size); }
std::string Location::getRoot() const { return (this->root); }
std::string Location::getCgiPass() const { return (this->cgi_pass); }

/********************************* SETEURS ************************************
******************************************************************************/
void Location::setMethods(std::string methods) { this->methods = methods; }
void Location::setName(std::string name) { this->name = name; }
void Location::setPort(std::string port) { this->port = port; }
void Location::setHost(std::string host) { this->host = host; }
void Location::setIndex(std::string index) {
	this->index = index; 
}

void Location::setCgiPass(std::string cgi_pass) { this->cgi_pass = cgi_pass; }

void Location::setAutoIndex(std::string autoindex) { 
	std::string pathAutoIndex;
	if (autoindex == "on")
	{
		this->autoindex = true;
		pathAutoIndex = this->root + this->name;
	}
	else
		pathAutoIndex = "";
	this->pathAutoIndex = pathAutoIndex;
}

void Location::setErrorPage(std::string error_page) { this->error_page = error_page; }
void Location::setErrorCode(int error_code) { this->error_code = error_code; }
void Location::setBodySize(int body_size) { this->body_size = body_size; }
void Location::setRoot(std::string root) { this->root = root; }

/******************************* CHECK ERRORS *********************************
******************************************************************************/
void Location::check_listen(std::string content) { 	
	std::size_t j = 0;
	std::string host;

	for (; content[j]; j++)
	{
		if (content[j] == ':')
			break;
		host += content[j];
	}
	setHost(host);

	std::string port;
	for (j += 1; content[j]; j++)
		port += content[j];
	for (j = 0; port[j]; j++)
	{
		if (!(isdigit(port[j])))
  		{			
			std::cerr << "Error, port is not valid\n";
			exit(EXIT_FAILURE);
		}
	}
	setPort(port);
}

void Location::check_root(std::string content) {
	std::vector<std::string> root = cpp_split(content, ' ');
	if (root.size() > 1)
	{
		std::cerr << "Error, root has to be a single file\n";
		exit(EXIT_FAILURE);
	}
	setRoot(content);
}

void Location::check_index(std::string content) {
	std::vector<std::string> index = cpp_split(content, ' ');
	if (index.size() > 1)
	{
		std::cerr << "Error, index has to be a single file\n";
		exit(EXIT_FAILURE);
	}
	if (content.length() > 0)
		content = this->root + this->name + content;
	setIndex(content);
}

void Location::check_error_page(std::string content) {
	std::vector<std::string> vec_error_page = cpp_split(content, ' ');

	if (vec_error_page.size() > 2)
	{
		std::cerr << "Error, error_page has to be a single file\n";
		exit(EXIT_FAILURE);
	}
	std::string error_code = vec_error_page[0].c_str();
	if (atoi(error_code.c_str()) < 400 || atoi(error_code.c_str()) > 599)
	{
		std::cerr << "Error, error code has to be 404\n";
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

void Location::check_cgi_pass(std::string content) {
	if (content.compare("./srcs/CGI/scripts/php-cgi") != 0)
	{
		std::cerr << "Error, cgi_pass not valid\n";
		exit(EXIT_FAILURE);
	}
	setCgiPass(content);
}

void Location::check_auto_index(std::string content) {
	if (content.compare("on") != 0 && content.compare("off") != 0)
	{			
		std::cerr << "Error, autoindex has to be either on or off\n";
		exit(EXIT_FAILURE);
	}
	setAutoIndex(content);
}

void Location::check_location_name(std::string content) { 
	int i = content.length() - 1;
	while (i >= 0 && (content[i] == '{' || content[i] == ' ' || content[i] == '\t' ))
		i--;
	int j = i;
	while (j > 0 && content[j] && (content[j] != ' ' || content[j] != '\t'))
		j--;
	std::string new_content = content.substr(j, i - j + 1);
	std::size_t find = new_content.find(".");
	if (new_content[0] != '/' && find == std::string::npos)
		new_content = "/" + new_content;
	if (new_content[new_content.length() - 1] != '/' && find == std::string::npos)
		new_content += '/';
	setName(new_content);
}

void Location::check_allow_methods(std::string content) {
	std::vector<std::string> methods = cpp_split(content, ' ');
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

void Location::check_client_max_body_size(std::string content) {
	std::string cut = first_word(content, 0);
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

void Location::check_each_element(std::string content, int i)
{
	content = cut_whitespace(content);
	switch (i) 
	{
		case 0 : check_listen(content);
			break;
		case 1 : check_root(content);
			break;
		case 2 : check_index(content);
			break;
		case 3 : check_allow_methods(content);
			break;
		case 4 : check_error_page(content);
			break;
		case 5 : check_client_max_body_size(content);
			break;
		case 6 : check_auto_index(content);
            break;
		case 7 : check_cgi_pass(content);
            break;
		case 9 : check_location_name(content);
			break;
	}
}

/******************************** UTILS ***************************************
******************************************************************************/
std::string Location::cut_whitespace(std::string str)
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

std::string Location::first_word(std::string str, int i)
{
	int j = 0;
	std::string new_str;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (i != 0)
    	j = i;
	while (str[j] && (str[j] != ' ' && str[j] != '\t'))
	{
		new_str += str[j];
		j++;
	}
	return new_str;
}

std::vector<std::string> Location::cpp_split(std::string str, char sep)
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

std::string Location::check_semicolon(std::string content) {
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

/******************************** PRINT ***************************************
******************************************************************************/
void Location::printLocation()
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
		std::cout << "body_size: " << body_size << "\n\n\n";
}
