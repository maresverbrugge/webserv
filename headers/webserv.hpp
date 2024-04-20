/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:01:32 by fkoolhov          #+#    #+#             */
/*   Updated: 2024/04/20 14:04:08 by fkoolhov         ###   ########.fr       */
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
# define YELLOW "\033[33m"
# define BOLD "\033[1m"
# define RESET "\033[0m"

enum e_methods
{
	GET,
	POST,
	DELETE
};

#endif