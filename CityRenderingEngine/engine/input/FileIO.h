/*
 * Author: Rodrigo Castro Azevedo
 * Date: 17/05/2014
 *
 * Description: The FileIO class is an utility class to help manage all file operations. It has functions to read,
 * write and manage files on the disk. It's instance-less, and all file operations throughout the engine should use it.
 */

#pragma once

#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class FileIO {
public:

    /*
     * Reads a text file and return its contents, organized in a vector, where each string is a line of the file. If
     * the file cannot be opened or read, it will just return an empty string vector. The maximum size that a single
     * line can have to be correctly read by this function is 1024. Any lines larger than 1024 characters will be
     * cropped, and only the first 1024 characters will be returned.
     */
    static std::vector<std::string> readTextFile(std::string fileName);

private:

    FileIO(void) {}
    ~FileIO(void) {}
};