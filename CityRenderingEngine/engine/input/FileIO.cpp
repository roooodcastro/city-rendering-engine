#include "FileIO.h"

std::vector<std::string> FileIO::readTextFile(std::string fileName) {
    std::vector<std::string> lines = std::vector<std::string>();
    std::ifstream file(fileName);
    if (file.is_open()) {
        char lineBuffer[1024];
        while(!file.eof()) {
            file.getline(lineBuffer, 1024);
            lines.emplace_back(lineBuffer);
        }
    }
    return lines;
}