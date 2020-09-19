#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <iomanip>

#include "lexer.hpp"
#include "lexeme.hpp"

int main(int argc, const char *argv[]) {
    std::ifstream fin;
    std::vector<Lexeme> lexemes;
    if (argc != 2) { //not enough arguments
        std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
        std::terminate();
    } else {
        openFile(argv[1], fin); //Open file
        if (!fin.is_open()) { //File opening error
            std::cerr << "Could not open file: " << argv[0] << std::endl;
            std::terminate();
        }
    }

    lexer(fin, lexemes);
    fin.close();
}

void openFile(std::string fileName, /*return*/ std::ifstream &file) {
    try {
        file.exceptions(std::ios::badbit | std::ios::failbit);
        file.open(fileName);
    } catch (const std::ios::failure & ex) {
        if (!file.eof()) {
            std::cerr << "openFile(): Error opening or reading from " << fileName << std::endl
                << "\tError code:  " << ex.code() << std::endl
                << "\tMeaning:     " << ex.what() << std::endl;
        }
    }
}

void lexer(std::ifstream &file, std::vector<Lexeme> &lexemes) {
    const std::string sepChars = "'(){}[],.:;";
    const std::string opChars = "*+-=/><%";

    State state = Start;
    std::string currentLex = "";
    std::cout << std::left << std::setw(12) << "TOKENS" << std::setw(6) << "" << std::setw(12) << "Lexemes" << std::endl;
    try {
        while (char c = file.peek()) {
            switch(state) { //switch based on current state
                case Start: //Starting state
                    //Transitions
                    if (isalpha(c)) { //Identifier
                        currentLex += c;
                        state = Identifier;
                    } else if (isdigit(c)) { //Int
                        currentLex += c;
                        state = Int;
                    } else if (sepChars.find(c) != std::string::npos) { //Seperator
                        currentLex += c;
                        state = Separator;
                    } else if (opChars.find(c) != std::string::npos) { //Operator
                        currentLex += c;
                        state = Operator;
                    } else if (c == '!') { //Comment
                        state = Comment;
                    }
                    break;
                case Identifier:
                    //Transitions
                    if (isalpha(c) || isdigit(c) || c == '$') { //Same
                        currentLex += c;
                        state = Identifier;
                    } else if (isKeyword(currentLex)) { //Keyword
                        file.unget();
                        state = Keyword;
                    } else { //Restart
                        std::cout << std::left << std::setw(12) << "Identifier" << std::setw(6) << "=" << std::setw(12) << currentLex << std::endl;
                        currentLex = "";
                        file.unget();
                        state = Start;
                    }
                    break;
                case Keyword:
                    //Restart
                    std::cout << std::left << std::setw(12) << "Keyword" << std::setw(6) << "=" << std::setw(12) << currentLex << std::endl;
                    currentLex = "";
                    file.unget();
                    state = Start;
                    break;
                case Separator:
                    //Restart
                    std::cout << std::left << std::setw(12) << "Separator" << std::setw(6) << "=" << std::setw(12) << currentLex << std::endl;
                    currentLex = "";
                    file.unget();
                    state = Start;
                    break;
                case Operator:
                    //Restart
                    std::cout << std::left << std::setw(12) << "Operator" << std::setw(6) << "=" << std::setw(12) << currentLex << std::endl;
                    currentLex = "";
                    file.unget();
                    state = Start;
                    break;
                case Float:
                    //Transitions
                    if (isdigit(c)) { //Same
                        currentLex += c;
                        state = Float;
                    } else {
                        std::cout << std::left << std::setw(12) << "Float" << std::setw(6) << "=" << std::setw(12) << currentLex << std::endl;
                        currentLex = "";
                        file.unget();
                        state = Start;
                    }
                    break;
                case Int:
                    //Transitions
                    if (isdigit(c)) { //Same
                        currentLex += c;
                        state = Int;
                    } else if (c == '.') { //Float
                        currentLex += c;
                        state = Float;
                    } else { //Restart
                        std::cout << std::left << std::setw(12) << "Integer" << std::setw(6) << "=" << std::setw(12) << currentLex << std::endl;
                        currentLex = "";
                        file.unget();
                        state = Start;
                    }
                    break;
                case Comment:
                    //Transitions
                    if (c != '!') { //Same
                        state = Comment;
                    } else { //Restart
                        state = Start;
                    }
                    break;
                case Unknown: //String doesn't match any known token types
                    break;
            }
            
            if (c != EOF) file.get();
            else break;
        }
    } catch (const std::ios::failure & ex) {
        std::cerr << "lexer(): Error reading from file." << std::endl
                << "\tError code:  " << ex.code() << std::endl
                << "\tMeaning:     " << ex.what() << std::endl;
    }
}

bool isKeyword(std::string str) {
    bool isKeyword = false;
    const std::string keywords[] = {"int", "float", "bool", "true", "false", "if", "else", "then", "endif", "while", "whileend", "do", "doend", "for", "forend", "input", "output", "and", "or", "not"};
    for (auto k : keywords) {
        if (str.compare(k) == 0) isKeyword = true;
    }
    return isKeyword;
}