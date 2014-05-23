#include "Shader.h"

Shader::Shader(std::string shaderName, std::string vertexFilename, std::string fragmentFilename) : 
    Resource(shaderName.c_str()) {
    this->vertexFilename = vertexFilename;
    this->fragmentFilename = fragmentFilename;
    this->geometryFilename = "";
    this->tessCtrlFilename = "";
    this->tessEvalFilename = "";
}

Shader::~Shader(void) {}

void Shader::load() {
    Renderer *renderer = Naquadah::getRenderer();
    if (renderer != nullptr) {
        std::string vertexCode = FileIO::mergeLines(FileIO::readTextFile(vertexFilename));
        std::string fragmentCode = FileIO::mergeLines(FileIO::readTextFile(fragmentFilename));
        std::string geometryCode = FileIO::mergeLines(FileIO::readTextFile(geometryFilename));
        std::string tessCtrlCode = FileIO::mergeLines(FileIO::readTextFile(tessCtrlFilename));
        std::string tessEvalCode = FileIO::mergeLines(FileIO::readTextFile(tessEvalFilename));

        this->program = renderer->createShaderProgram();

        renderer->compileShader(program, GL_VERTEX_SHADER, vertexCode.c_str());
        renderer->compileShader(program, GL_FRAGMENT_SHADER, fragmentCode.c_str());
        if (geometryCode.size() > 0)
            renderer->compileShader(program, GL_GEOMETRY_SHADER, geometryCode.c_str());
        if (tessCtrlCode.size() > 0 && tessEvalCode.size() > 0) {
            renderer->compileShader(program, GL_TESS_CONTROL_SHADER, tessCtrlCode.c_str());
            renderer->compileShader(program, GL_TESS_EVALUATION_SHADER, tessEvalCode.c_str());
        }

        setDefaultAttributes();
        if (renderer->linkProgram(program))
            loaded = true;
    } else
        loaded = false;
}

void Shader::unload() {
    glDeleteProgram(program);
}

/*
This function should set up which generic attribute attaches to which
input variable of the vertex shader. I always make my vertex shaders
use the same basic names (i.e "position" for positions...) so that it
becomes trivial to attach vertex data to shaders, without having to
mess around with layout qualifiers in the shaders themselves etc.

This is up to you, though, you wouldn't be docked any marks for doing
the vertex shader / VBO connection a different way!
*/
void Shader::setDefaultAttributes()	{
    Renderer *renderer = Naquadah::getRenderer();
    if (renderer != nullptr) {
        renderer->bindAttributeLocation(program, LOC_VERTEX_BUFFER, "position");
        renderer->bindAttributeLocation(program, LOC_COLOUR_BUFFER, "colour");
        renderer->bindAttributeLocation(program, LOC_UV_MAP_BUFFER, "uv_map");
        renderer->bindAttributeLocation(program, LOC_NORMAL_BUFFER, "normal");
        //renderer->bindAttributeLocation(program, LOC_TANGENT_BUFFER, "tangent");
    }
}