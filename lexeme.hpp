#ifndef LEXEME_HPP
#define LEXEME_HPP

#include <string>

class Lexeme {
    private:
    std::string _lex;
    std::string _token;

    public:
    void lex(std::string lex);
    std::string lex();

    void token(std::string token);
    std::string token();
};

#endif //LEXEME_HPP