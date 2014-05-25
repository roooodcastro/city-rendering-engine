#include "Shader.h"

Shader::Shader(std::string shaderName, std::string vertexFilename, std::string fragmentFilename) : 
    Resource(shaderName) {
    this->vertexFilename = vertexFilename;
    this->fragmentFilename = fragmentFilename;
    this->geometryFilename = "";
    this->tessCtrlFilename = "";
    this->tessEvalFilename = "";
}

Shader::~Shader(void) {}

void Shader::load() {

    std::string vertexString = std::string(FileIO::mergeLines(FileIO::readTextFile(vertexFilename), '\n'));
    std::string fragmentString = std::string(FileIO::mergeLines(FileIO::readTextFile(fragmentFilename), '\n'));
    std::string geometryString = std::string(FileIO::mergeLines(FileIO::readTextFile(geometryFilename), '\n'));
    std::string tessCtrlString = std::string(FileIO::mergeLines(FileIO::readTextFile(tessCtrlFilename), '\n'));
    std::string tessEvalString = std::string(FileIO::mergeLines(FileIO::readTextFile(tessEvalFilename), '\n'));
    const char *vertexCode = vertexString.c_str();
    const char *fragmentCode = fragmentString.c_str();
    const char *geometryCode = geometryString.c_str();
    const char *tessCtrlCode = tessCtrlString.c_str();
    const char *tessEvalCode = tessEvalString.c_str();



    Renderer *renderer = Naquadah::getRenderer();
    if (renderer != nullptr) {
        std::string vertexCode = FileIO::mergeLines(FileIO::readTextFile(vertexFilename));
        std::string fragmentCode = FileIO::mergeLines(FileIO::readTextFile(fragmentFilename));
        std::string geometryCode = FileIO::mergeLines(FileIO::readTextFile(geometryFilename));
        std::string tessCtrlCode = FileIO::mergeLines(FileIO::readTextFile(tessCtrlFilename));
        std::string tessEvalCode = FileIO::mergeLines(FileIO::readTextFile(tessEvalFilename));

        this->program = glCreateProgram();

        compileShader(program, GL_VERTEX_SHADER, vertexCode.c_str());
        compileShader(program, GL_FRAGMENT_SHADER, fragmentCode.c_str());
        if (geometryCode.size() > 0)
            compileShader(program, GL_GEOMETRY_SHADER, geometryCode.c_str());
        if (tessCtrlCode.size() > 0 && tessEvalCode.size() > 0) {
            compileShader(program, GL_TESS_CONTROL_SHADER, tessCtrlCode.c_str());
            compileShader(program, GL_TESS_EVALUATION_SHADER, tessEvalCode.c_str());
        }

        setDefaultAttributes();
        if (linkProgram(program))
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
        bindAttributeLocation(program, LOC_VERTEX_BUFFER, "position");
        bindAttributeLocation(program, LOC_COLOUR_BUFFER, "colour");
        bindAttributeLocation(program, LOC_UV_MAP_BUFFER, "uv_map");
        bindAttributeLocation(program, LOC_NORMAL_BUFFER, "normal");
        //renderer->bindAttributeLocation(program, LOC_TANGENT_BUFFER, "tangent");
    }
}

bool Shader::compileShader(GLuint program, GLenum shaderType, const char *shaderCode) {
    if (glIsProgram(program)) {
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderCode, nullptr);
        glCompileShader(shader);
        GLint status = GL_FALSE, infoLogLength = 10;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_TRUE) {
            glAttachShader(program, shader);
            //logOpenGLError("ATTACH_" + getShaderTypeName(shaderType));
            return true;
        } else {
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            std::vector<char> infoLog(infoLogLength, 'X');
            glGetShaderInfoLog(shader, infoLogLength, nullptr, &infoLog[0]);
            std::cerr << "OpenGL Error compiling " << getShaderTypeName(shaderType) << " of Shader Program '" <<
                name << "':" << std::endl;
            std::cerr << &infoLog[0] << std::endl;
            //logOpenGLError("COMPILE_" + getShaderTypeName(shaderType));
        }
    }
    return false;
}

bool Shader::linkProgram(GLuint program) {
    if (glIsProgram(program)) {
        glLinkProgram(program);
        GLint status = GL_FALSE, len = 10;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == GL_TRUE) return true;
        // If we can't link, print the error to the console
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len, 'X');
        glGetProgramInfoLog(program, len, nullptr, &log[0]);
        std::cerr << &log[0] << std::endl;
        //logOpenGLError("LINK_PROGRAM");
    }
    return false;
}

void Shader::bindAttributeLocation(GLuint program, GLuint location, std::string attrName) {
    glBindAttribLocation(program, location, attrName.c_str());
}

bool Shader::operator==(Shader &other) {
    if (&other)
        return this->program == other.program;
    return false;
}

bool Shader::operator!=(Shader &other) {
    return !(*this == other);
}