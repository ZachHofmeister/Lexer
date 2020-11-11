#include <iostream>
#include <fstream>
#include <string>

#include "main.hpp"
#include "lexer.hpp"
#include "lexeme.hpp"

int main(int argc, const char *argv[]) {
    std::ifstream fin;
    if (argc >= 2 && argc <= 3 && openFile(argv[1], fin)) { //file arg is provided and file (fin) is open
		//Get lexemes
		auto lexemes = lexicalAnalysis(fin);
        fin.close();
		//Print lexemes
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

template <typename TFile>
bool openFile(std::string fileName, /*return*/ TFile &file) {
    file.open(fileName);
    return file.is_open();
}