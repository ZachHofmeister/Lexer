#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "lexeme.hpp"

enum State : short {Start, Identifier, Keyword, Separator, Operator, Float, Int, Comment, Unknown};

Lexeme lexer(std::ifstream &file);
bool isKeyword(std::string str);
bool openFile(std::string fileName, /*return*/ std::ifstream &file);
bool openFile(std::string fileName, /*return*/ std::ofstream &file);
void printLexemes(std::ostream &stream, std::vector<Lexeme> &lexemes);

#endif //LEXER_HPP