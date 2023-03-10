/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mede-sou <mede-sou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 17:57:37 by mede-sou          #+#    #+#             */
/*   Updated: 2023/03/10 14:53:49 by mede-sou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"

std::string cut_whitespace(std::string str)
{
	int j = str.length();
	int i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	while (str[j] && j < i)
		j--;
	std::string new_str = str.substr(i, j - i + 1);
	return new_str;
}

std::string first_word(std::string str, int i)
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

std::string check_semicolon(std::string content) {
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

std::vector<std::string> cpp_split(std::string str, char sep)
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

std::string extract_content(std::vector<std::string>::iterator it, int i, std::string keyword)
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
	if (i != 6)
		content = check_semicolon(content);
	return (content);
}

void check_brackets(std::vector<std::string> vec)
{
	std::vector<std::string>::iterator it = vec.begin();
	int count = 0; int count2 = 0;
	for (it = vec.begin(); it != vec.end();++it)
	{
		if (!(*it).empty())
		{
			std::size_t finding = (*it).find("{");
			std::size_t finding2 = (*it).find("}");
			std::size_t finding3 = (*it).find("#");
			if (finding != std::string::npos && finding < finding3)
				count++;
			if (finding2 != std::string::npos && finding2 < finding3)
				count2++;
		}
	}
	if ((count + count2) % 2 != 0)
	{
		std::cerr << "Error: Brackets missing\n";
		exit(EXIT_FAILURE);
	}
}

bool check_syntax(std::vector<std::string>::iterator it, std::string str)
{
	std::size_t found = it->find("{");
	std::size_t i = 0;
	if (found == std::string::npos)
		return false;

	while ((*it)[i] == ' ' || (*it)[i] == '\t')
		i++;
	std::size_t j = i + str.length();
	for (; j < found && (*it)[j]; j++)
	{
		if ((*it)[j] != ' ' && (*it)[j] != '\t')
			return false;
	}
	for (j = found + 1; j < (*it).length(); j++)
	{
		if ((*it)[j] != ' ' && (*it)[j] != '\t')
			return false;
	}
	return true;
}

void check_doublon_kw(std::vector<std::string> vec_kw)
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