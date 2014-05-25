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
#include <cstdlib>
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

    /*
     * Creates and compiles a new shader, attaching it to program. The shaderType defines the type of the shader that
     * will be created, and shaderCode is the actual shader text. Returns true if the shader was created and compiled
     * successfully, or false if OpenGL raised an error in any of these stages, and the shader couldn't be compiled.
     */
    bool compileShader(GLuint program, GLenum shaderType, const char *shaderCode);

    /*
     * Links the shader program. Returns true if the program was successfully linked, or false otherwise.
     */
    bool linkProgram(GLuint program);

    /*
     * Binds an attribute name to a location that is basically a GLuint that OpenGL shaders can use to map which
     * variable in the shader it's supposed to send data to.
     */
    void bindAttributeLocation(GLuint program, GLuint location, std::string attrName);

    /*
     * Returns a string with the name of the type of the shader. If it's a vertex shader, it'll return "VERTEX_SHADER",
     * and so on. If it's not any type of supported shader, it'll return "INVALID_SHADER_TYPE"
     */
    std::string getShaderTypeName(GLenum shaderType) {
        switch(shaderType) {
        case GL_VERTEX_SHADER:
            return "VERTEX_SHADER";
            break;
            case GL_FRAGMENT_SHADER:
                return "FRAGMENT_SHADER";
            break;
            case GL_GEOMETRY_SHADER:
                return "GEOMETRY_SHADER";
            break;
            case GL_TESS_CONTROL_SHADER:
                return "TESSELATION_CONTROL_SHADER";
            break;
            case GL_TESS_EVALUATION_SHADER:
                return "TESSELATION_EVALUATION_SHADER";
            break;
            default:
                return "INVALID_SHADER_TYPE";
        }
    }

    bool operator==(Shader &other);
    bool operator!=(Shader &other);

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