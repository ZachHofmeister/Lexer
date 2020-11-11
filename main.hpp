#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>

template<typename TFile>
bool openFile(std::string fileName, /*return*/ TFile &file); //file: ifstream or ofstream

#endif //MAIN_HPP