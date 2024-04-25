/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:01:32 by fkoolhov          #+#    #+#             */
/*   Updated: 2024/04/25 12:46:59 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <cstdlib>
# include <vector>
# include <map>
# include <string>
# include <array>
# include <stack>
# include <unordered_set>
# include <fstream>
# include <sstream>
# include <memory>

# define GREEN "\033[32m"
# define BLUE "\033[34m"
# define RED "\033[31m"
# define YELLOW "\033[33m"
# define BOLD "\033[1m"
# define RESET "\033[0m"
# define DEFAULT_CONFIG "./config/default.conf"

enum e_methods
{
	GET,
	POST,
	DELETE
};

#endif