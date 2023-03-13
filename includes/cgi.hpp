/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amanasse <amanasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 13:46:45 by amanasse          #+#    #+#             */
/*   Updated: 2023/03/13 12:49:36 by amanasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <map>
#include "cgi.hpp"
#include "colors.h"

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <dirent.h>
#include <bitset>

// #include <Request.hpp>

class Request;

class Cgi {

	public:

		Cgi();
		Cgi(Cgi const &cpy);
		Cgi & operator=(Cgi const &cpy);
		~Cgi();

		int									InitEnvCgi(void);
		int									CheckTypes(void);
		void								ClearArray(void);
		int									InterpretCgi(Request &request_r, std::map<std::string, std::string>);
		void								ClearEnvCgi(void);
		int									CheckPathInfo(void);
		int									CheckExtension(void);
		int									ClearCgi(int error, char** tab);
		std::map<std::string, std::string> 	getMap(void);
		char**								getMapChar(void);
		
		int 								Interprate_html(void);
		int 								Interprate_php_CGI(void);
		int 								Interprate_img(void);
	
		std::string							html_to_string(std::string value_error);
		std::string							CompleteString(std::string html);
		const char*							CompleteString_img(char *html, size_t ContentSize);
		int									Exec(void);
		int									init_cgi(void);

		std::string							getIndex(void);
		std::string							getHtml(void);
		const char *						getImg(void);
		std::string							getAllBody(void);
		std::string							getContentType(void);
		std::string							getContentLength(void);
		std::string							getContentBody(void);
		std::string							getStatus(void);
		std::string							getPathInfo(void);
		std::string							getMethod(void);
		bool								getAutoIndex(void);
		std::string							getPathAutoIndex(void);
		std::string							getErrorPage(void);
		int									getErrorCode(void);
		int									getMaxBodySize(void);
	
		void								setIndex(std::string Index);
		void								setHtml(std::string html);
		void								setImg(const char * img);
		void								setAllBody(std::string AllBody);
		void								setStatus(std::string Status);
		void								setPathInfo(std::string PathInfo);
		void								setMethod(std::string Method);
		void								setContentType(std::string ContentType);
		void								setContentLength(std::string ContentLengt);
		void								setContentBody(std::string ContentBody);
		void								setAutoIndex(bool AutoIndex);
		void								setPathAutoIndex(std::string PathAutoIndex);
		void								setErrorPage(std::string ErrorPage);
		void								setErrorCode(int ErrorCode);
		void								setMaxBodySize(int MaxBodySize);

		void 								GenerateAutoIndex(std::string path);

		
	private:
		char**								_env_cgi;
		const char *						_img_to_Html;
		std::string							_Html_in_String;
		std::map<std::string, std::string>	_mimes;
		std::map<std::string, std::string>	_map_cgi_env;	

		std::string							_Index;
		std::string							_AllBody;
		std::string							_ContentType;
		std::string							_ContentLength;
		std::string							_ContentBody;
		std::string							_Status;
		std::string							_PathInfo;
		std::string							_Method;
		bool								_AutoIndex;
		std::string							_PathAutoIndex;
		std::string							_ErrorPage;
		int									_ErrorCode;
		int									_MaxBodySize;
		bool								_img;
};


#endif
