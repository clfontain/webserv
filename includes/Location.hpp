/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mede-sou <mede-sou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 17:48:07 by mede-sou          #+#    #+#             */
/*   Updated: 2023/03/06 13:25:31 by mede-sou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <fstream>
#include <istream>
#include <cstring>
#include <stdexcept>
#include <cstddef>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <cmath>

class Location {
   public : 
    Location();
    Location(Location const &cpy);
    Location & operator=(Location const &assign);
    ~Location();

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

    void printLocation();

    void setMethods(std::string methods);
    void setName(std::string name);
    void setPort(std::string port);
    void setHost(std::string host);
    void setIndex(std::string index);
    void setErrorPage(std::string error_page);
    void setErrorCode(int error_code);
    void setBodySize(int body_size);
    void setRoot(std::string root);
    void setAutoIndex(std::string autoindex);
    void setCgiPass(std::string cgi_pass);

    void check_each_element(std::string content, int i);
    void check_location_name(std::string content);
    void check_allow_methods(std::string content);
    void check_listen(std::string content);
    void check_root(std::string content);
    void check_index(std::string content);
    void check_auto_index(std::string content);
    void check_error_page(std::string content);
    void check_cgi_pass(std::string content);

    void check_client_max_body_size(std::string content);
    std::vector<std::string> cpp_split(std::string str, char sep);
    std::string cut_whitespace(std::string str);
    std::string first_word(std::string str, int i);
    std::string check_semicolon(std::string content);

    private:
        std::string methods;
        std::string name;
        std::string port;
        std::string host;
        std::string index;
        std::string error_page;
        int         error_code;
        int         body_size;
        std::string root;
        bool        autoindex;
        std::string pathAutoIndex;
        std::string cgi_pass;

};


#endif