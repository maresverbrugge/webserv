/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:49:39 by felicia           #+#    #+#             */
/*   Updated: 2024/04/22 15:27:20 by fkoolhov         ###   ########.fr       */
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

bool check_for_brackets(std::vector<std::string> words, std::stack<char>& brackets)
{
	if (words.size() == 1 && words[0] == "{")
	{
		brackets.push('{');
		if (brackets.size() != 1)
			throw std::runtime_error("Too many brackets encountered.");
		return true;
	}
	else if (words.size() == 1 && words[0] == "}")
	{
		brackets.pop();
		return true;
	}
	return false;
}