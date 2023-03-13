#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Parsing.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "cgi.hpp"

class Request {

	public:
		Request();
		~Request();
		Server *find_port( void );
		void request_to_cgi( void );
		bool GetBmethod( void) const;
		void find_location();
		void check_Method();
		void check_Protocol();
		void check_location();
		void parsing_request( void );
		void parsing_header(void);
		void parsing_body(void);
		int	check_request();
		void print_request( void );
		void Process_request();
		void SetReq_location(std::vector<std::string> &loc);

		void check_startLine(std::string startLine);
		void find_host(std::string findHostLine);
		void find_content_info(std::string findContentLine);
		void check_doublon_keywords(std::vector<std::string> vec_kw);

		//*************************************************************//
		//								SETTER						   //
		//*************************************************************//
		void SetMethod(std::string &str);
		void SetPort(std::string &str);
		void SetHost(std::string &str);
		void SetR_Location(std::string &str);
		void SetProtocol(std::string &str);
		void SetParsing(Parsing *parsing);
		void SetRequest(std::string request);
		void SetServer(Server *serv);
		void SetResponse(std::string Response);
		void SetHeader(std::string Header);
		void SetBody(std::string Body);
		void SetContentLength(int ContentLength);

		//*************************************************************//
		//								GETTER						   //
		//*************************************************************//

		std::string GetMethod( void) const;
		std::string GetPort( void) const;
		std::string GetHost( void) const;
		std::string GetR_Location( void) const;
		std::string GetProtocol( void) const;
		Parsing *GetParsing( void) const;
		Server *GetServer( void) const;
		std::string GetResponse( void) const;
		std::string GetHeader( void) const;
		std::string GetBody( void) const;
		int 		GetContentLength( void) const;
		//std::string GetRequest(char *request);


	private:
	std::string Method;
	std::string Response;
	std::string Port;
	std::string Host;
	std::string R_Location;
	std::string request;
	std::string Protocol;
	std::string Header;
	std::string Body;
	int			ContentLength;

	std::vector<std::string> request_split_line;
	std::vector<std::string> request_split_space;
	std::vector<std::string> location_split;
	std::vector<std::string> req_location;
	//std::vector< Location* > vec_location;
	bool b_method;
	bool bad_request;
	bool loc_found;
	Location *loc;
	Cgi cgi;
	Parsing *parsing;
	Server *serv;
	int	index_server;
};

std::vector<std::string> cpsplit(std::string str, const char *sep);
void cpsplit_header(std::string str, const char *sep);
void print_vector(std::vector<std::string> vec);

#endif

