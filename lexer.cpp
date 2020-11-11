#include <string>
#include <vector>
#include <array>
#include <utility>
#include <regex>
#include <fstream>
#include <iomanip>

#include "lexeme.hpp"

#include "lexer.hpp"

std::vector<Lexeme> lexicalAnalysis(std::ifstream &file) { //TODO maybe refactor into a class?
	const std::array<std::pair<std::string, std::string>, 8> tokenRegex {{
		//Token			Matching Regular Expression
        {"Keyword",		"(int|float|bool|true|false|if|else|then|endif|while|whileend|do|doend|for|forend|input|output|and|or|not)"},
        {"Identifier",	"([a-zA-Z][a-zA-Z\\d$]*)"},
        {"Real",		"([+\\-]?\\d*\\.\\d+)"},
        {"Int",			"(\\d+)"},
        {"Separator",	"([[\\]{}(),.;:])"},
        {"Operator",	"([=+\\-\\/%*<>])"},
        {"Comment",		"(![^!]*!)"},
        {"Unknown",		"(\\S)"}
	}};
	//Unions the token regexs into a single regex
	std::string combined = "";
	for (auto it = tokenRegex.begin(); it < tokenRegex.end(); ++it) {
		combined += it->second + (it + 1 < tokenRegex.end()? "|" : ""); // Adds the regex followed by union (|) if not last element
	}
	std::regex regex(combined);
	//Perform lexical analysis
	std::string fileStr = readFile(file); //Read file to string
	std::vector<Lexeme> lexemes;
	Lexeme temp;
	while(lexer(tokenRegex, regex, fileStr, temp)) {
		lexemes.push_back(temp);
	}
	return lexemes;
}

bool lexer(std::array<std::pair<std::string, std::string>, 8> tokenRegex, std::regex regex, std::string &fileStr, /*return*/ Lexeme &lex) {
	//Find a match
	std::smatch match;
	std::regex_search(fileStr, match, regex); //Returns the first match between fileStr and regex
	size_t tokenIndex; //index in tokenRegex of the regex that was matched
	for (tokenIndex = 0; tokenIndex < match.size(); ++tokenIndex) {
		if (!match.str(tokenIndex+1).empty()) break;
	}
	lex = Lexeme(match.str(), tokenRegex.at(tokenIndex).first); //Build the lexeme
	fileStr = match.suffix().str(); //Move past this lexeme in the file string
	//Validate, to return
	bool isValid = !match.empty();
	if (lex.token().compare("Comment") == 0) { //Skip comments by recursion
		isValid = lexer(tokenRegex, regex, fileStr, lex);
	}
	return isValid;
}

std::string readFile(std::ifstream &file) {
    std::string line, fileStr = "";
    while (std::getline(file, line)) {
		fileStr += line + "\n";
	}
	return fileStr;
}

void printLexemes(std::ostream &stream, std::vector<Lexeme> &lexemes) {
    stream << std::left << std::setw(12) << "TOKENS" << std::setw(6) << "" << std::setw(12) << "LEXEMES" << std::endl;
    for (auto lex : lexemes) {
        stream << std::left << std::setw(12) << lex.token() << std::setw(6) << "=" << std::setw(12) << lex.lex() << std::endl;
    }
}