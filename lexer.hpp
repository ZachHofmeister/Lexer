#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <array>
#include <utility>
#include <regex>

#include "lexeme.hpp"

std::vector<Lexeme> lexicalAnalysis(std::ifstream &file); //Performs lexical analysis on a file, returns vector of lexemes
bool lexer(std::array<std::pair<std::string, std::string>, 8> tokenRegex, std::regex regex, std::string &fileStr, /*return*/ Lexeme &lex);
std::string readFile(std::ifstream &file);
void printLexemes(std::ostream &stream, std::vector<Lexeme> &lexemes);

#endif //LEXER_HPP