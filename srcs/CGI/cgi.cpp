/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amanasse <amanasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 13:24:03 by amanasse          #+#    #+#             */
/*   Updated: 2023/03/13 13:07:48 by amanasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>


//////////////////////////////////////////////////////
//                                                  //
//                    Constructors                  //
//                                                  //
//////////////////////////////////////////////////////


Cgi::Cgi(void)
{
	this->_img = false;
    return;
}

Cgi::~Cgi(void)
{
    return;
}

//////////////////////////////////////////////////////
//                                                  //
//                FONCTONS MEMBRES                  //
//                                                  //
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//                                                  //
//                CONVERSION HTML                   //
//                                                  //
//////////////////////////////////////////////////////

std::string Cgi::html_to_string(std::string value_error)
{
	std::string		line;
	std::string		content;
	if (value_error == "200 OK")
	{
		std::map<std::string, std::string> 	envMap = this->getMap();
		std::string 						value = this->getPathInfo();
		std::cout << BLUE << value << RESET << std::endl;
		std::ifstream						file(value.c_str());
		
		if (file.is_open() == true)
		{
			while (getline(file, line))
			{
				content += line + "\n";
			}
			file.close();
		}
		else
		{
			std::cout << "ERROR\nCannot open" << std::endl;
		}
		return (content);
	}
	if (value_error == "200 AUTO")
	{
		std::map<std::string, std::string> 	envMap = this->getMap();
		std::string 						value = this->_PathInfo + "autoindex.html";
		std::cout << BLUE << value << RESET << std::endl;
		std::ifstream						file(value.c_str());
		
		if (file.is_open() == true)
		{
			while (getline(file, line))
			{
				content += line + "\n";
			}
			file.close();
		}
		else
		{
			std::cout << "ERROR\nCannot open" << std::endl;
		}
		return (content);
	}
	else if (value_error == "404")
	{
		std::string 			value = "./srcs/CGI/errors/404.html";
		std::ifstream			file(value.c_str());
		if (file.is_open() == true)
		{
			while (getline(file, line))
			{
				content += line + "\n";
			}
			file.close();
		}
		else
		{
			std::cout << "ERROR\nCannot open" << std::endl;
		}
		return (content);
	}
	else if (value_error == "400")
	{
		std::string 			value = "./srcs/CGI/errors/400.html";
		std::ifstream			file(value.c_str());
		if (file.is_open() == true)
		{
			while (getline(file, line))
			{
				content += line + "\n";
			}
			file.close();
		}
		else
		{
			std::cout << "ERROR\nCannot open" << std::endl;
		}
		return (content);
	}
	else if (value_error == "405")
	{
		std::string 			value = "./srcs/CGI/errors/405.html";
		std::ifstream			file(value.c_str());
		if (file.is_open() == true)
		{
			while (getline(file, line))
			{
				content += line + "\n";
			}
			file.close();
		}
		else
		{
			std::cout << "ERROR\nCannot open" << std::endl;
		}
		return (content);
	}
	else if (value_error == "409")
	{
		std::string 			value = "./srcs/CGI/errors/409.html";
		std::ifstream			file(value.c_str());
		if (file.is_open() == true)
		{
			while (getline(file, line))
			{
				content += line + "\n";
			}
			file.close();
		}
		else
		{
			std::cout << "ERROR\nCannot open" << std::endl;
		}
		return (content);
	}
	else
	{
		std::string 			value = "body_php";
		std::ifstream			file(value.c_str());
		if (file.is_open() == true)
		{
			while (getline(file, line))
			{
				content += line + "\n";
			}
			file.close();
		}
		else
		{
			std::cout << "ERROR\nCannot open" << std::endl;
		}
		return (content);
	}
	return (content);    
}


//////////////////////////////////////////////////////
//                                                  //
//            EXECUTION DU SCRIPT PHP-CGI           //
//                                                  //
//////////////////////////////////////////////////////

int Cgi::Exec(void)
{
	char **env = this->getMapChar();

	std::string path_cgi = "srcs/CGI/cgi-bin/php-cgi";
	std::string path = this->getPathInfo(); /*METTRE LE PATH INFO DE LA REQUETE (getPathInfo)*/

	char **tab = new char *[3];
	tab[0] = new char[path_cgi.size() + 1]; 
	std::strcpy(tab[0], path_cgi.c_str());
	tab[1] = new char[path.size() + 1]; 
	std::strcpy(tab[1], path.c_str());
	tab[2] = NULL;

	int pid = fork();
	int pipefd[2];

	if (pipe(pipefd) != 0)
		return (-1);

	std::string path_one = "body_php";
	std::string path_two = this->getPathInfo(); /*METTRE LE PATH INFO DE LA REQUETE (getPathInfo) --> request_r.Get_Path();*/

	int tmp_file = ::open(path_one.c_str(), O_CREAT | O_RDWR | O_TRUNC, 00755);

	if (pid == -1)
	{
		std::cout << "Error\n" << std::endl;
		return (-1);
	}
	else if (pid == 0)
	{
		std::cout << "Je suis l'enfant\n" << std::endl;        
		
		if (!(chdir(path_two.c_str())))
			return (-1);
				close(pipefd[1]);
				
		if (dup2(pipefd[0], 0) == -1)
			return (-1);
			
		if (dup2(tmp_file, 1) == -1)
			return (-1);
		
		close(pipefd[0]);
		
		if (execve(tab[0], tab, env) < 0)
			return(std::cout << "ERROR EXEC" << std::endl, -1);
	}
	else
	{
		std::cout << "Je suis le parent\n" << std::endl;
		close(pipefd[0]);
		close(pipefd[1]);

		int status = 0;
		if (waitpid(pid, &status, 0) == -1)
			return (-1);
		
		if (WIFEXITED(status) && WEXITSTATUS(status))
			return (-1);
	}
	delete []tab[0];
	delete []tab[1];
	delete []tab;
	return (200);
}

//////////////////////////////////////////////////////
//                                                  //
//               CHECKING DES ERREURS               //
//                                                  //
//////////////////////////////////////////////////////

int Cgi::CheckTypes(void)
{
	std::string		line;
	std::string		content;
	std::ifstream	file("srcs/CGI/mimes/mimes.types");
	
	if (file.is_open() == true)
	{
		while (getline(file, line))
		{
			content += line + "\n";
    		std::istringstream iss(content);
    		std::string line2;

		    while (getline(iss, line2)) 
			{
	        	std::string value = line2.substr(0, line2.find_first_of(' '));
	        	std::string key = line2.substr(line2.find_last_of(' ') + 1);
	        	this->_mimes[key] = value; 
   			}
		}
		file.close();
	}
	else
	{
		std::cout << "ERROR\nCannot open MIMES" << std::endl;
		return (-1);
	}
    for (std::map<std::string, std::string>::iterator it = _map_cgi_env.begin(); it != _map_cgi_env.end(); it++) 
	{
        std::string value = it->second;
        bool found = false;
        for (std::map<std::string, std::string>::iterator it2 = _mimes.begin(); it2 != _mimes.end(); it2++) 
		{
            if (it2->second == value) 
			{
                found = true;
                break;
            }
        }
        if (found)
			return (200);
    }	
	return (-1);
}


int Cgi::CheckExtension(void)
{
	std::string value = this->getPathInfo();
	std::string::size_type dotPos = value.find_last_of('.');
	std::string extension = value.substr(dotPos);
	if (extension == ".html")
		return (1);
	else if (extension == ".php")
		return (2);
	else if (extension == ".png" || extension == ".jpg" || extension == ".gif")
		return (3);
	return (-1);
}

//////////////////////////////////////////////////////
//                                                  //
//                 INIT ENV CGI			            //
//                                                  //
//////////////////////////////////////////////////////

int	Cgi::InitEnvCgi(void)
{
	if (!this->_ContentLength.empty())
		this->_map_cgi_env["CONTENT_LENGTH"] = this->getContentLength();
	this->_map_cgi_env["METHOD"] = this->getMethod();
	if (this->_Method == "POST")
		this->_map_cgi_env["CONTENT_TYPE"] = this->getContentType(); //application/x-www-form-urlencoded // this->getContentType()
	else
		this->_map_cgi_env["CONTENT_TYPE"] = "text/html";
	this->_map_cgi_env["REDIRECT_STATUS"] = this->getStatus();
	if (this->_map_cgi_env["REDIRECT_STATUS"] != "200 OK")
		return (-1);
	this->_map_cgi_env["PATH_INFO"] = this->getPathInfo();
	
	std::cout << BLUE << this->getContentBody() << RESET << std::endl;
	// this->_map_cgi_env["SCRIPT_NAME"] = "./scrs/CGI/cgi-bin/php-cgi";		 	
	// this->_map_cgi_env["SERVER_PROTOCOL"] = "HTTP/1.1";

	// this->_map_cgi_env["QUERY_STRING"] = "q=chat&lang=fr";
	// this->_map_cgi_env["SERVER_NAME"] = "localhost";

	
    std::map<std::string, std::string>::iterator it;
	this->_env_cgi = (char**)malloc(sizeof(char*) * (this->_map_cgi_env.size() + 1));
	if (!this->_env_cgi)
		return (-1);
	int i = 0;
	it = this->_map_cgi_env.begin();
	while (i < (int)this->_map_cgi_env.size())
	{
		std::string tmp = it->first + "=" + it->second + '\0';
		this->_env_cgi[i] = strdup((char*)tmp.c_str());
		i++;
		++it;
	}
	this->_env_cgi[i] = NULL;
	i = 0;
	while (this->_env_cgi[i])
	{
		std::cout << BLUE << this->_env_cgi[i] << RESET << std::endl;
		i++;
	}

	return (0);
}

//////////////////////////////////////////////////////
//                                                  //
//                 HTML TO STRING		            //
//                                                  //
//////////////////////////////////////////////////////

std::string Cgi::CompleteString(std::string html)
{
	std::string tmp;
	size_t size = 0; 
	std::ostringstream oss;
	if (this->_img == true)
	{
		// // Ouvrir l'image en binaire en mode lecture
    	// std::ifstream image(this->_PathInfo, std::ios::binary);

    	// // Vérifier si le fichier est ouvert avec succès
    	// if (!image) 
        // 	std::cerr << "Impossible d'ouvrir le fichier" << std::endl;

    	// // Récupérer la taille de l'image en octets
    	// image.seekg(0, std::ios::end);
    	// int taille = image.tellg();
    	// image.close();
	
    	// // Afficher la taille de l'image en octets
    	// std::cout << "Taille de l'image en octets : " << taille << std::endl;
		size = html.size();
		std::cout << RED << size << RESET << std::endl;
    	oss << size;
		tmp = "HTTP/1.1 " + this->getStatus() + "\r\nContent-Type: image/png\r\nContent-Length: 4868"/* + oss.str() */+ "\r\n\n";
		this->_img = false;
	}
	else
	{
		size = html.size();
    	oss << size;
		tmp = "HTTP/1.1 " + this->getStatus() + "\nContent-Type: text/html\nContent-Length: " + oss.str() + "\n\n";
	}
	tmp += html;
	return (tmp);
}


int Cgi::Interprate_img(void)
{
	std::cout << "je suis la";
	std::string status = this->getStatus();
	std::string html = html_to_string(status);

	std::ifstream input(_PathInfo.c_str(), std::ios::binary);

	if (!input.is_open())
		return (-1);
	//calcul la taille
	size_t ContentSize;
	input.seekg(0, std::ios::end);
	ContentSize = input.tellg();
	std::cout << BOLD_YELLOW << "Content size : " << ContentSize << std::endl;
	// input.seekg(0, std::ios::beg);
	char *html_str = new char[ContentSize];
	
	//taille long converti en char*
	//malloc la taille de s'quon a ouvert au dessus
	//avec squon a ouvert(input) on read  dans content_bin
	input.read(html_str, ContentSize);
	std::cout << html_str << RESET << std::endl;
	if (!(html.empty()))
	{
		html = CompleteString(html);
		this->setHtml(html);
	}
	else
		return (-1);
	return (0);
}

int Cgi::Interprate_html(void)
{
	std::string status = this->getStatus();
	std::string html = html_to_string(status);
	if (!(html.empty()))
	{
		html = CompleteString(html);
		this->setHtml(html);
	}
	else
		return (-1);
	return (0);
}

int Cgi::Interprate_php_CGI(void)
{
	std::cout<<"j'execute un script php-CGI" << std::endl;
	if (this->Exec() == 404)
	{
		std::string html = html_to_string("404");
		if (!(html.empty()))
		{
			html = CompleteString(html);
			this->setHtml(html);
		}
		return (-1);
	}
	std::string html = html_to_string("1");
	if (!(html.empty()))
	{
		html = CompleteString(html);
		this->setHtml(html);
	}
	else
		return (-1);
	return(0);
}

int Cgi::init_cgi(void)
{
	//STATUS DIFFERENT DE 200 OK

    if (this->InitEnvCgi() == -1)
	{
		if (this->getStatus() != "200 OK")
		{
			std::string html = html_to_string(this->getStatus());
			if (!(html.empty()))
			{
				html = CompleteString(html);
				this->setHtml(html);
			}
		}
		else
		{
			std::string html = html_to_string("404");
			if (!(html.empty()))
			{
				html = CompleteString(html);
				this->setHtml(html);
			}
		}
		std::cout << RED << "ERROR\ninit" << RESET <<std::endl;
		this->ClearArray();
    	return (-1);
	}
    struct stat fileStat;
	
    if (stat(this->_PathInfo.c_str(), &fileStat) < 0) 
	{
        std::string html = html_to_string("404");
		if (!(html.empty()))
		{
			html = CompleteString(html);
			this->setHtml(html);
		}
		std::cout << RED << "ERROR\nfile" << RESET <<std::endl;
		this->ClearArray();
        return (-1);
    }
    if (S_ISDIR(fileStat.st_mode)) 
	{
    	std::cout << "est un dossier\n";
		std::cout << GREEN << "PATH : " << this->_PathInfo << RESET << std::endl;
		std::cout << GREEN << "Autodindex : " << this->_AutoIndex << RESET << std::endl;
		if(this->_PathInfo[this->_PathInfo.size() - 1] == '/' && this->_AutoIndex == true)
		//AUTOINDEX
		{
			GenerateAutoIndex(this->_PathInfo);
			std::string html = html_to_string("200 AUTO");
			if (!(html.empty()))
			{
				html = CompleteString(html);
				this->setHtml(html);
				std::string autoIndex = this->_PathInfo + "autoindex.html"; 
				const char* autoindex02 = autoIndex.c_str();
			    remove(autoindex02);
				this->ClearArray();
				return (0);
			}
		}
		else
		{
			std::cout << BLUE << "METTRE L'INDEX.HTML SELON FICHIER DE CONFIG" << RESET << std::endl;
			this->_PathInfo = this->getIndex();
			std::string html = html_to_string("200 OK");
			if (!(html.empty()))
			{
				html = CompleteString(html);
				this->setHtml(html);
			}
			this->ClearArray();
			return (0);
		}
	}
	else if (S_ISREG(fileStat.st_mode)) 
	{
        std::cout << "est un fichier\n";
    } 
	else 
	{
        std::cout << "n'est ni un dossier ni un fichier\n";
		std::string html = html_to_string("404");
		if (!(html.empty()))
		{
			html = CompleteString(html);
			this->setHtml(html);
		}
		this->ClearArray();
        return -1;
    }
    if (this->getMethod() == "DELETE")
	{
		std::string html = html_to_string("409");
		if (!(html.empty()))
		{
			html = CompleteString(html);
			this->setHtml(html);
		}
		this->ClearArray();
    	return (-1);
	}
	if (this->CheckTypes() == -1)
	{
		std::cout << RED << "ERROR\nTYPES" << RESET <<std::endl;
		std::string html = html_to_string("404");
		if (!(html.empty()))
		{
			html = CompleteString(html);
			this->setHtml(html);
		}
		this->ClearArray();
    	return (-1);
	}
	if (this->CheckExtension() == 1)
	{
		if (this->Interprate_html() == -1)
		{
			this->ClearArray();
			return (-1);
		}
	}
	else if (this->CheckExtension() == 2)
	{
		if (this->Interprate_php_CGI() == -1)
		{
			this->ClearArray();
			return (-1);
		}		
	}
	else if (this->CheckExtension() == 3)
	{
		this->_img = true;
		if (this->Interprate_img() == -1)
		{
			this->ClearArray();
			return (-1);
		}		
	}
    this->ClearArray();
	return (0);
}



void Cgi::GenerateAutoIndex(std::string path) {
    DIR* dir = opendir(path.c_str());

    if (dir == NULL) 
	{
        std::cerr << "Error: cannot open the directory\n";
        exit(EXIT_FAILURE);
    }

    std::string html_autoindex = "<html><body><ul>";

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
	{
        if (entry->d_name[0] != '.')
		{
            html_autoindex += "<li>";
            html_autoindex += entry->d_name;
            html_autoindex += "</li>";
        }
    }
    html_autoindex += "</ul></body></html>";

    std::ofstream file((path + "/autoindex.html").c_str());
    file << html_autoindex;
	
    file.close();

    closedir(dir);
}

void Cgi::ClearArray(void)
{
	int i = 0;
	if (this->_env_cgi)
	{
		while (this->_env_cgi[i])
		{
			free (this->_env_cgi[i]);
			i++;
		}
		free (this->_env_cgi);
	}
}

//////////////////////////////////////////////////////
//                                                  //
//                    GETERS                        //
//                                                  //
//////////////////////////////////////////////////////

char** Cgi::getMapChar(void)
{
	return (this->_env_cgi);	
}

std::map<std::string, std::string> Cgi::getMap(void)
{
	return (this->_map_cgi_env);	
}

std::string Cgi::getIndex(void)
{
	return (this->_Index);	
}

std::string Cgi::getHtml(void)
{
	return (this->_Html_in_String);	
}

std::string Cgi::getAllBody(void)
{
	return (this->_AllBody);	
}

std::string Cgi::getPathInfo(void)
{
	return (this->_PathInfo);	
}

std::string Cgi::getStatus(void)
{
	return (this->_Status);	
}

std::string Cgi::getMethod(void)
{
	return (this->_Method);	
}

std::string Cgi::getContentType(void)
{
	return (this->_ContentType);	
}

std::string Cgi::getContentBody(void)
{
	return (this->_ContentBody);	
}

std::string Cgi::getContentLength(void)
{
	return (this->_ContentLength);	
}

bool Cgi::getAutoIndex(void)
{
	return (this->_AutoIndex);	
}

std::string Cgi::getPathAutoIndex(void)
{
	return (this->_PathAutoIndex);	
}

std::string Cgi::getErrorPage(void)
{
	return (this->_ErrorPage);	
}

int Cgi::getErrorCode(void)
{
	return (this->_ErrorCode);	
}

int Cgi::getMaxBodySize(void)
{
	return (this->_MaxBodySize);	
}

//////////////////////////////////////////////////////
//                                                  //
//                    SETERS                        //
//                                                  //
//////////////////////////////////////////////////////

void Cgi::setIndex(std::string Index)
{
	this->_Index = Index;
}

void Cgi::setHtml(std::string html)
{
	this->_Html_in_String = html;
}

void Cgi::setStatus(std::string Status)
{
	this->_Status = Status;
}

void Cgi::setAllBody(std::string AllBody)
{
	this->_AllBody = AllBody;
}

void Cgi::setContentType(std::string ContentType)
{
	this->_ContentType = ContentType;
}

void Cgi::setContentBody(std::string ContentBody)
{
	this->_ContentBody = ContentBody;
}

void Cgi::setContentLength(std::string ContentLength)
{
	this->_ContentLength = ContentLength;
}

void Cgi::setPathInfo(std::string PathInfo)
{
	this->_PathInfo = PathInfo;
}

void Cgi::setMethod(std::string Method)
{
	this->_Method = Method;
}

void Cgi::setAutoIndex(bool AutoIndex) 
{
	this->_AutoIndex = AutoIndex;
}

void Cgi::setPathAutoIndex(std::string PathAutoIndex)
{
	this->_PathAutoIndex = PathAutoIndex;
}

void Cgi::setErrorPage(std::string ErrorPage)
{
	this->_ErrorPage = ErrorPage;
}

void Cgi::setErrorCode(int ErrorCode)
{
	this->_ErrorCode = ErrorCode;
}

void Cgi::setMaxBodySize(int MaxBodySize)
{
	this->_MaxBodySize = MaxBodySize;
}