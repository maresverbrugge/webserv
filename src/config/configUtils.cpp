/* ************************************************************************* */
/*      ##       ##      ## ##       ##      ## ##       ##      ##          */
/*       ##     ####    ##   ##     ####    ##   ##     ####    ##           */
/*        ##  ##   ##  ##     ##  ##   ##  ##     ##  ##   ##  ##            */
/*         ####     ####       ####     ####       ####     ####             */
/*          ##       ##         ##       ##         ##       ##              */
/*                                                                           */
/*           WONDERFUL            WEBSERV           WONDERTEAM               */
/*                                                                           */
/*      FELICIA KOOLHOVEN      FLEN HUISMAN       MARES VERBRUGGE            */
/*          fkoolhov             fhuisman             mverbrug               */
/*                                                                           */
/*          Codam Coding College        part of 42 network                   */
/*                            April - May 2024                               */
/* ************************************************************************* */

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

void config_error_message(std::string message)
{
	std::cerr << RED BOLD "Config error: " RESET << message << std::endl;
}