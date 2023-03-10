/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mede-sou <mede-sou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 11:19:55 by mede-sou          #+#    #+#             */
/*   Updated: 2023/02/24 17:26:10 by mede-sou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <fstream>
#include <istream>
#include <cstring>
#include <stdexcept>
#include <cstddef>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <map>
#include <cmath>
#include "Location.hpp"

class Server {
    public:
        Server();
        Server(Server const &cpy);
        Server & operator=(Server const &assign);
        ~Server();

        std::string getMethods() const;
        std::string getName() const;
        std::string getPort() const;
        std::string getHost() const;
        std::string getIndex() const;
        std::string getErrorPage() const;
        int         getErrorCode() const;
        int         getBodySize() const;
        std::string getRoot() const;
        bool        getAutoIndex() const;
        std::string getPathAutoIndex() const;
        std::string getCgiPass() const;
        std::vector<Location *> getVectorLocation() const ;

        void setMethods(std::string methods);
        void setName(std::string name);
        void setPort(std::string port);
        void setHost(std::string host);
        void setIndex(std::string index);
        void setErrorCode(int error_code);
        void setErrorPage(std::string error_page);
        void setBodySize(int body_size);
        void setRoot(std::string root);
        void setAutoIndex(std::string autoindex);
        void setCgiPass(std::string cgi_pass);

        void printServer();
        void printLocationVector();

        std::vector<std::string>::iterator createNewLocation(std::vector<std::string>::iterator it);
        void copy_server_to_location(Location &location);

        void check_each_element(std::string content, int i);
        void check_server_name(std::string content);
        void check_allow_methods(std::string content);
        void check_listen(std::string content);
        void check_root(std::string content);
        void check_index(std::string content);
        void check_auto_index(std::string content);
        void check_error_page(std::string content);
        void check_client_max_body_size(std::string content);
        void check_cgi_pass(std::string content);

        std::vector<std::string> cppsplit(std::string str, char sep);
        std::string cutwhitespace(std::string str);
        std::string firstword(std::string str, int i);
        std::string extractcontent(std::vector<std::string>::iterator it, int i, std::string keyword);
        std::string checksemicolon(std::string content);
        void check_doubl_kw(std::vector<std::string> vec_kw);
        
        
        std::vector<std::string > vec_file;

    private:
        std::string methods;
        std::string name;
        std::string port;
        std::string host;
        std::string index;
        int         error_code;
        std::string error_page;
        int         body_size;
        std::string root;
        bool        autoindex;
        std::string pathAutoIndex;
        std::string cgi_pass;
        
        int count_location;
    	bool sublocation;

        std::vector< Location* > vec_location;

};
#endif
