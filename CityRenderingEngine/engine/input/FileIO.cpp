#include "FileIO.h"

std::vector<std::string> FileIO::readTextFile(std::string fileName) {
    std::vector<std::string> lines = std::vector<std::string>();
    std::ifstream file;
    file.open(fileName);
    if (file.is_open()) {
        std::string lineBuffer;
        while(!file.eof()) {
            getline(file, lineBuffer);
            lines.emplace_back(lineBuffer);
        }
    }
    return lines;
}

std::string FileIO::mergeLines(std::vector<std::string> lines, char delimiter) {
    if (lines.size() > 0) {
        std::string result = lines[0];
        for (int i = 1; i < lines.size(); i++) {
            result += delimiter + lines[i];
        }
        return result;
    }
    return "";
}

bool FileIO::fileExists(const std::string &filePath) {
    if (FILE *file = fopen(filePath.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}