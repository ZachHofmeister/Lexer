#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <iomanip>

#include "lexer.hpp"
#include "lexeme.hpp"

int main(int argc, const char *argv[]) {
    std::ifstream fin;
    if (argc >= 2 && argc <= 3 && openFile(argv[1], fin)) { //All good (file arg provided and file (fin) is open)
        //Perform lexical analysis
        std::vector<Lexeme> lexemes;
        while(!fin.eof()) {
            lexemes.push_back(lexer(fin));
        }
        fin.close();
        //Output
        printLexemes(std::cout, lexemes); //Print to std::cout
        if (argc == 3) {
            std::ofstream fout;
            if (openFile(argv[2], fout)) printLexemes(fout, lexemes); //Print to output file
            else std::cerr << "main(): Could not open file: " << argv[2] << std::endl; //Error: Problem opening output file
            fout.close();
        }
    } else if (argc < 2 || argc > 3) { //Error: Not enough / too many arguments
        std::cerr << "Usage: " << argv[0] << " filename <outputFilename(optional)>" << std::endl;
    } else { //Error: Problem opening input file
        std::cerr << "main(): Could not open file: " << argv[1] << std::endl;
    }

    return 0;
}

Lexeme lexer(std::ifstream &file) {
    const std::string sepChars = "'(){}[],.:;";
    const std::string opChars = "*+-=/><%";

    State state = Start;
    bool done = false;
    std::string temp = "";
    Lexeme lex;
    try {
        while (char c = file.peek()) {
            switch(state) { //switch based on current state
                case Start: //Starting state
                    temp += c;

                    if (isspace(c)) { //Same
                        temp = "";
                        state = Start;
                    } else if (isalpha(c)) { //Identifier
                        state = Identifier;
                    } else if (isdigit(c)) { //Int
                        state = Int;
                    } else if (c == '.') { //Float
                        state = Float;
                    } else if (sepChars.find(c) != std::string::npos) { //Seperator
                        state = Separator;
                    } else if (opChars.find(c) != std::string::npos) { //Operator
                        state = Operator;
                    } else if (c == '!') { //Comment
                        state = Comment;
                    } else { //Unknown
                        state = Unknown;
                    }
                    break;
                case Identifier:
                    if (isalpha(c) || isdigit(c) || c == '$') { //Same
                        temp += c;
                        state = Identifier;
                    } else if (isKeyword(temp)) { //Keyword
                        file.unget();
                        c = 0;
                        state = Keyword;
                    } else { //End
                        lex = Lexeme(temp, "Identifier");
                        done = true;
                    }
                    break;
                case Keyword: //End
                    lex = Lexeme(temp, "Keyword");
                    done = true;
                    break;
                case Separator: //End
                    lex = Lexeme(temp, "Separator");
                    done = true;
                    break;
                case Operator: //End
                    lex = Lexeme(temp, "Operator");
                    done = true;
                    break;
                case Int:
                    if (isdigit(c)) { //Same
                        temp += c;
                        state = Int;
                    } else if (c == '.') { //Float
                        temp += c;
                        state = Float;
                    } else { //End
                        lex = Lexeme(temp, "Int");
                        done = true;
                    }
                    break;
                case Float:
                    if (isdigit(c)) { //Same
                        temp += c;
                        state = Float;
                    } else if (temp.length() == 1 && temp.at(0) == '.') { //Separator
                        file.unget();
                        c = 0;
                        state = Separator;
                    } else { //End
                        lex = Lexeme(temp, "Float");
                        done = true;
                    }
                    break;
                case Comment:
                    if (c != '!') { //Same
                        state = Comment;
                    } else { //(re)Start
                        temp = "";
                        state = Start;
                    }
                    break;
                case Unknown: //End
                    lex = Lexeme(temp, "Unknown");
                    done = true;
                    break;
            } //end switch
            if (c != EOF && !done) file.get();
            else break;
        } //end while
    } catch (const std::ios::failure & ex) {
        std::cerr << "lexer(): Error reading from file." << std::endl
                << "\tError code:  " << ex.code() << std::endl
                << "\tMeaning:     " << ex.what() << std::endl;
    }

    return lex;
}

bool isKeyword(std::string str) {
    bool isKeyword = false;
    const std::string keywords[] = {"int", "float", "bool", "true", "false", "if", "else", "then", "endif", "while", "whileend", "do", "doend", "for", "forend", "input", "output", "and", "or", "not"};
    for (auto k : keywords) {
        if (str.compare(k) == 0) isKeyword = true;
    }
    return isKeyword;
}

bool openFile(std::string fileName, /*return*/ std::ifstream &file) {
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
    return file.is_open();
}

bool openFile(std::string fileName, /*return*/ std::ofstream &file) {
    file.open(fileName);
    return file.is_open();
}

void printLexemes(std::ostream &stream, std::vector<Lexeme> &lexemes) {
    stream << std::left << std::setw(12) << "TOKENS" << std::setw(6) << "" << std::setw(12) << "Lexemes" << std::endl;
    for (auto l : lexemes) {
        stream << std::left << std::setw(12) << l.token() << std::setw(6) << "=" << std::setw(12) << l.lex() << std::endl;
    }
}