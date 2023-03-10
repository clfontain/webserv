
#include "Request.hpp"

Request::Request() : b_method(false), bad_request(false), loc(NULL)
{
 
}

Request::~Request()
{

}



//*************************************************************//
//								SETTER						   //
//*************************************************************//

void Request::SetMethod(std::string &str)
{
	this->Method = str;
}

void Request::SetPort(std::string &str)
{
	this->Port = str;
}

void Request::SetHost(std::string &str)
{
	this->Host = str;
}

void Request::SetR_Location(std::string &str)
{
	this->R_Location = str;
}

void Request::SetProtocol(std::string &str)
{
	this->Protocol = str;
}

void Request::SetReq_location(std::vector<std::string> &loc)
{
	this->req_location = loc;
}

void Request::SetParsing(Parsing *parsing)
{
	this->parsing = parsing;
}

void Request::SetServer(Server *serv)
{
	this->serv = serv;
}

void Request::SetRequest(char *request)
{
	this->request = request;
}

void Request::SetResponse(std::string Response)
{
	this->Response = Response;
}

void Request::SetBody(std::string Body)
{
	this->Body = Body;
}

void Request::SetHeader(std::string Header)
{
	this->Header = Header;
}

void Request::SetContentLength(int ContentLength)
{
	this->ContentLength = ContentLength;
}

//*************************************************************//
//								GETTER						   //
//*************************************************************//

std::string Request::GetMethod( void) const
{
	return (this->Method);
}

std::string Request::GetPort( void) const
{
	return (this->Port);
}

std::string Request::GetHost( void) const
{
	return (this->Host);
}

std::string Request::GetR_Location( void) const
{
	return (this->R_Location);
}

std::string Request::GetProtocol( void) const
{
	return (this->Protocol);
}

Parsing *Request::GetParsing( void) const
{
	return (this->parsing);
}

Server *Request::GetServer( void) const
{
	return (this->serv);
}

bool Request::GetBmethod( void) const
{
	return (this->b_method);
}

std::string Request::GetResponse( void) const
{
	return (this->Response);
}

std::string Request::GetBody( void) const
{
	return (this->Body);
}

std::string Request::GetHeader( void) const
{
	return (this->Header);
}

int Request::GetContentLength( void) const
{
	return (this->ContentLength);
}

void Request::request_to_cgi( void )
{
	this->cgi.setMethod(this->GetMethod());
	if (this->GetContentLength())
	{
		std::ostringstream oss;
    	oss << this->GetContentLength();
		std::string length = oss.str();
		this->cgi.setContentLength(length);
	}
	this->cgi.setContentBody(this->Body);
	this->cgi.setAllBody(this->Header + this->Body);
	this->cgi.setPathInfo(this->loc->getRoot() + this->R_Location);
	if (this->cgi.getStatus().empty())
		this->cgi.setStatus("200 OK");

	this->cgi.setErrorCode(this->loc->getErrorCode());
	this->cgi.setErrorPage(this->loc->getErrorPage());
	this->cgi.setMaxBodySize(this->loc->getBodySize());
	this->cgi.setIndex(this->loc->getIndex());

	this->cgi.setAutoIndex(this->loc->getAutoIndex());
	this->cgi.setPathAutoIndex(this->loc->getPathAutoIndex());

	this->cgi.init_cgi();
	this->SetResponse(this->cgi.getHtml());
}

void Request::check_Protocol()
{
	if (this->GetProtocol() != "HTTP/1.1")
	{
		this->cgi.setStatus("505");
		std::cout << "ECHEC PROTOCOL" << std::endl;
	}
}

void Request::check_Method()
{
	size_t pos;
	if (this->loc == NULL)
		pos = this->serv->getMethods().find(this->GetMethod());
	else
		pos = this->loc->getMethods().find(this->GetMethod());
	if (pos == std::string::npos)
	{
		this->cgi.setStatus("405");
		std::cout << "ECHEC METHOD" << std::endl;
	}
}

void Request::check_location()
{
	check_Method();
	check_Protocol();
}

void Request::find_location()
{
	size_t pos;
	this->loc_found = false;
	size_t len = this->location_split.size() - 1;

	std::vector<Location *>::iterator it;
	std::vector< Location* > loc = serv->getVectorLocation();
	for (len = this->location_split.size() - 1;len < serv->getVectorLocation().size(); len--)
	{
		for (it = loc.begin(); it != loc.end(); ++it)		
		{
			pos = (*it)->getName().find(this->location_split[len]);
			if (pos != std::string::npos)
			{
				this->loc_found = true;
				this->loc = (*it);
				break;
			}
		}
	}
	if (this->loc_found == false)
	{
		for (it = loc.begin(); it != loc.end(); ++it)		
		{
			if ((*it)->getName() == "/")
			{
				this->loc_found = true;
				this->loc = (*it);
				break;
			}
		}
	}
}

Server *Request::find_port( void )
{
	std::vector<Server *> serv = this->parsing->getVectorServer();
	std::vector<Server *>::iterator it;
	int i;
	i = 0;
	for (it = serv.begin(); it != serv.end(); ++it)
	{
		if ((*it)->getPort() == this->Port)
		{
			this->index_server = i;
			break;
		}	
		i++;
	}
	return ((*it));
}

int Request::check_request()
{
	if (this->Method.empty() || this->R_Location.empty() || this->Protocol.empty() 
		|| this->Host.empty() || this->Port.empty())
	{
		this->bad_request = true;
		this->cgi.setStatus("400");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void Request::find_content_info(std::string findContentLine)
{
	std::vector<std::string> tmp = cpsplit(findContentLine, " ");
	size_t found = findContentLine.find("\r\nContent-Type:");
	if (found != std::string::npos)
		this->cgi.setContentType(tmp[1]);
}

void Request::find_host(std::string findHostLine)
{
	std::vector<std::string> tmp;
	size_t found = findHostLine.find("Host:");
	if (found != std::string::npos)
	{
		tmp = cpsplit(this->request_split_space[1], ":");
		this->SetHost(tmp[0]);
		this->SetPort(tmp[1]);
	}
}

void Request::check_startLine(std::string startLine)
{
	cpsplit_header(startLine, " ");
	if (startLine.size() > 1000)
	{
		this->cgi.setStatus("414");
		std::cerr << "Error: Request-URI Too Long\n";
	}
	this->SetMethod(this->request_split_space[0]);
	this->SetR_Location(this->request_split_space[1]);
	this->SetProtocol(this->request_split_space[2]);
}

void Request::check_doublon_keywords(std::vector<std::string> vec_kw)
{
	std::vector<std::string>::iterator it = vec_kw.begin();
	for (it = vec_kw.begin(); it != vec_kw.end(); ++it)
	{
		if (std::count(vec_kw.begin(), vec_kw.end(), *it) > 1)
		{
			this->cgi.setStatus("400");
			std::cerr << "Error: Bad Request\n";
		}
	}
}

void Request::parsing_header(void)
{
	std::vector<std::string> tmp;
	std::vector<std::string> vector_kw;

	this->request_split_line = cpsplit(this->request, "\r\n"); // a changer avec this->Header
	for (size_t j = 0; j < this->request_split_line.size(); j++)
	{
		if (this->request_split_line.size() > 10000 || 
			(this->request_split_line[0] == " " || this->request_split_line[0] == "\t"))
		{
			this->cgi.setStatus("400");
			std::cerr << "Error: Bad Request\n";
		}
		this->request_split_space = cpsplit(this->request_split_line[j], " ");

		if (j == 0 && this->request_split_space.size() == 3)
			check_startLine(this->request_split_line[j]);
		int len = this->request_split_space[0].length() - 2;
		if (isspace(this->request_split_space[0][len]) != 0 && j != 0)
		{
			this->cgi.setStatus("400");
			std::cerr << "Error: Bad Request\n";
		}
		vector_kw.push_back(this->request_split_space[0]);
		find_host(this->request_split_line[j]);
		find_content_info(this->request_split_line[j]);
		this->request_split_space.clear();
	}
	check_doublon_keywords(vector_kw);
}

void Request::parsing_request( void )
{
	parsing_header();
	this->location_split = cpsplit(this->GetR_Location(), "/");
}

void Request::Process_request()
{
	this->parsing_request();
	this->print_request();
	if (this->check_request() == EXIT_FAILURE)
	{
		this->cgi.setStatus("400");
		std::cout << "Error: invalid request" << std::endl;
		return;
	}	
	this->serv = this->find_port();
	this->find_location();
	this->check_location();
	this->request_to_cgi();
}

void Request::print_request( void )
{
	std::cout << "METHOD = " << this->Method << std::endl;
	std::cout << "REQUEST LOCATION = " << this->R_Location << std::endl;
	std::cout << "PROTOCOL = " << this->Protocol << std::endl;
	std::cout << "HOST = " << this->Host << std::endl;
	std::cout << "PORT = " << this->GetPort() << std::endl;
}
