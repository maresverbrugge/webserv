/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felicia <felicia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:49:39 by felicia           #+#    #+#             */
/*   Updated: 2024/04/18 18:50:54 by felicia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configuration.hpp"

std::vector<std::string> get_words_in_line(std::string line)
{
	std::istringstream stringstream(line);
	std::vector<std::string> words;
	std::string word;

	while (stringstream >> word)
		words.push_back(word);
	return (words);
}