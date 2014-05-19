/*
 * Authors: Rich Davison and Rodrigo Castro Azevedo
 * Date: 17/05/2014
 *
 * Description: Class to encapsulate OpenGL GLSL shader functionality, treating all of the shader stages as a single
 * coherent program.
 *
 * -_-_-_-_-_-_-_,------,   
 * _-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
 * -_-_-_-_-_-_-~|__( ^ .^) /
 * _-_-_-_-_-_-_-""  ""   
 *
 */

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "GL/glew.h"
#include "Model.h"

using namespace std;

class Shader {
public:

    Shader(const string &vertex, const string &fragment, const string &geometry = "", const string &tcs = "", const string &tes = "");
    ~Shader(void);

    GLuint  getShaderProgram() { return program; }

    string loadTextFile(string filename);

protected:

    GLuint program;
    // std::vector<ShaderAttribute*> *shaderAttributes;


    void SetDefaultAttributes();
};