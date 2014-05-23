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
#include "../input/FileIO.h"
#include "../Resource.h"
#include "Renderer.h"
#include "Model.h"

class Shader : public Resource {
public:

    static const GLuint LOC_VERTEX_BUFFER = 0;
    static const GLuint LOC_COLOUR_BUFFER = 1;
    static const GLuint LOC_UV_MAP_BUFFER = 2;
    static const GLuint LOC_NORMAL_BUFFER = 3;
    //static const GLuint LOC_TANGENT_BUFFER = 4;

    /*
     * Creates a new Shader, with the specified filenames for the shader files. ShaderName is the name of the shader
     * resource that will be created. Note that the actual OpenGL shaders won't be created here, but only when the
     * resource is loaded, using load().
     */
    Shader(std::string shaderName, std::string vertexFilename, std::string fragmentFilename);
    virtual ~Shader(void);

    /* This function should load the shader into OpenGL */
    virtual void load();

    /* This function should destroy the shader, unloading and releasing its data from OpenGL. */
    virtual void unload();

    /* Getters and setters for the filenames. */
    std::string getVertexFilename() { return vertexFilename; }
    std::string getFragmentFilename() { return fragmentFilename; }
    std::string getGeometryFilename() { return geometryFilename; }
    std::string getTessCtrlFilename() { return tessCtrlFilename; }
    std::string getTessEvalFilename() { return tessEvalFilename; }
    void setVertexFilename(std::string filename) { this->vertexFilename = filename; }
    void setFragmentFilename(std::string filename) { this->fragmentFilename = filename; }
    void setGeometryFilename(std::string filename) { this->geometryFilename = filename; }
    void setTessCtrlFilename(std::string filename) { this->tessCtrlFilename = filename; }
    void setTessEvalFilename(std::string filename) { this->tessEvalFilename = filename; }

    /* Returns the OpenGL ID of this Shader Program. */
    GLuint getShaderProgram() { return program; }

protected:

    /* The unique ID that OpenGL assigns for the Shader Program. */
    GLuint program;

    /* These variables hold the filname of the different Shaders that the program can have. */
    std::string vertexFilename;
    std::string fragmentFilename;
    std::string geometryFilename;
    std::string tessCtrlFilename;
    std::string tessEvalFilename;

    /**/
    // std::vector<ShaderAttribute*> *shaderAttributes;

    /* Sets some attributes necessary to tell OpenGL the names that we're going to use for some GLSL variables. */
    void setDefaultAttributes();
};