
#include "webserv.hpp"

void	print_vector(std::vector<std::string> vec)
{
	for(size_t i = 0; i < vec.size(); i++)
		std::cout << vec[i] << std::endl;
}

void	print_vector_m(std::vector<std::map<std::string, std::string> > vec_m)
{
	std::map<std::string, std::string>::iterator it;


	for(size_t i = 0; i < vec_m.size(); i++)
	{
		for(it = vec_m[i].begin(); it != vec_m[i].end(); ++it)
			{
    			std::cout << it->first << " " << it->second << " " << "\n";
			}
	}
}

std::vector<std::string> cpsplit(std::string str, const char *sep)
{
	std::vector<std::string> vec;
	std::string tmp;
	size_t pos;
	size_t i = 0;

	while (str[i] != 0)
	{
		pos = str.find(sep, i);
		if (i != pos)
		{
			tmp = str.substr(i, (pos - i));
			vec.push_back(tmp);
		}
		if (pos != std::string::npos)
			i = pos + strlen(sep);
		else
			break;
	}	
	return (vec);
}

void cpsplit_header(std::string str, const char *sep)
{
	std::vector<std::string> vec;
	std::string tmp;
	size_t pos;
	size_t i = 0;
	int count = 0;

	while (str[i] != 0)
	{
		pos = str.find(sep, i);
		if (i != pos)
		{
			tmp = str.substr(i, (pos - i));
			vec.push_back(tmp);
		}
		if (pos != std::string::npos)
		{
			count++;
			i = pos + strlen(sep);
		}
		else
			break;
	}
	if (count != 2)
	{
		// this->cgi.setStatus("414");
		std::cerr << "Error: invalid start line format/n";
	}
}


char *ft_putnbr(char *str, long int n)
{
	int len ;

	if (n >= 10)
	{
		ft_putnbr(str, (n / 10));
		ft_putnbr(str, (n % 10));
	}
	if (n < 10)
	{
		len = strlen(str);
		str[len]  = (n + 48);
	}
	return (str);
}

char *itoa(int n)
{
	int j, len;
	j = n;
	len = 1;
	if (n < 0)
		len = 2;
	while ((j / 10) != 0)
	{
		len++;
		j = (j / 10);
	}
	char *str;
	str = new char[len + 1];
	std::cout << len << std::endl;
	memset(str, 0, len + 1);
	str = ft_putnbr(str, n);
	return (str);
}
