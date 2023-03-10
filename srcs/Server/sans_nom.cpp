/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sans_nom.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfontain <cfontain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 12:18:31 by cfontain          #+#    #+#             */
/*   Updated: 2023/03/02 14:28:15 by cfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// std::vector<std::map<std::string, std::string> > tmp_v_loc( void )
// {
// 	std::map<std::string, std::string> map_v0;
// 	std::map<std::string, std::string> map_v1;
// 	std::map<std::string, std::string> map_v2;
// 	std::map<std::string, std::string> map_v3;
// 	std::vector<std::map<std::string, std::string> > pars_v;
// 	map_v0.insert(std::pair<std::string, std::string>("server_name", "webserv"));
// 	map_v1.insert(std::pair<std::string, std::string>("location", "/"));
// 	map_v1.insert(std::pair<std::string, std::string>("allow_methods", "GET"));
// 	map_v2.insert(std::pair<std::string, std::string>("location", "image"));
// 	map_v2.insert(std::pair<std::string, std::string>("allow_methods", "GET"));
// 	map_v3.insert(std::pair<std::string, std::string>("location", "picture"));
// 	pars_v.push_back(map_v0);
// 	pars_v.push_back(map_v1);
// 	pars_v.push_back(map_v2);
// 	pars_v.push_back(map_v3);
// 	//print_vector_m(pars_v);
// 	return (pars_v);
// }



// int request(s_server *server)
// {
// 	std::map<std::string, std::string>::iterator it;
// 	bool found = FALSE;
// 	server->pars_v = tmp_v_loc();
// 	server->req.SetMethod(server->v_req[0]);
// 	int i;
// 	size_t j;
// 	i = 0;
// 	i = server->v_req_path.size();
// 	if (i != 0)
// 	{
// 		for(j = 1; j < server->pars_v.size(); j++)
// 		{
// 			it = server->pars_v[j].end();
// 			it--;
// 			if (it->second == server->v_req_path[i - 1])
// 			{
// 				found = TRUE;
// 				//server->req.copy_info_location(server->pars_v[j]);
// 				break;
// 			}
// 		}
		
// 	}
// 	else
// 		std::cout << "IN ROOT"<< std::endl;
// 	if (found == TRUE)
// 		std::cout << "it : " << it->second << "path : " << server->v_req_path[i - 1] << std::endl;
// 	else
// 		std::cout << "NO LOCATION FOUND"<< std::endl;
// 	server->req.SetParsing(server->pars);
// 	server->req.find_port("8080");
// 	//std::cout << server->req.GetMethod() << std::endl;
// 	//std::cout << server->req.GetBmethod() << std::endl;
// 	return (0);
// }
