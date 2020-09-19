#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "lexeme.hpp"

enum State : short {Start, Identifier, Keyword, Separator, Operator, Float, Int, Comment, Unknown};

void openFile(std::string fileName, /*return*/ std::ifstream &file);
void lexer(std::ifstream &file, std::vector<Lexeme> &lexemes);
bool isKeyword(std::string str);

#endif //LEXER_HPP