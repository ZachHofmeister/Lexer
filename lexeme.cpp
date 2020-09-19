#include <string>

#include "lexeme.hpp"

void Lexeme::lex(std::string lex) { _lex = lex; }
std::string Lexeme::lex() { return _lex; }

void Lexeme::token(std::string token) { _token = token; }
std::string Lexeme::token() { return _token; }