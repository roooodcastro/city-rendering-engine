#include "Shader.h"

Shader::Shader(int name, const std::string &vertexFilename, const std::string &fragmentFilename) : 
    Resource(name) {
    this->vertexFilename = std::string(vertexFilename);
    this->fragmentFilename = std::string(fragmentFilename);
    this->geometryFilename = "";
    this->tessCtrlFilename = "";
    this->tessEvalFilename = "";
    this->shaderParameters = new std::vector<ShaderParameter*>();
}

Shader::~Shader(void) {
    for (auto it = shaderParameters->begin(); it != shaderParameters->end(); it++) {
        delete (*it);
    }
    shaderParameters->clear();
    delete shaderParameters;
    shaderParameters = nullptr;
}

void Shader::load() {
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
            valid = true;
    } else
        valid = false;
    loaded = true;
}

void Shader::unload() {
    // TODO: store the GLuint of the shaders to be able to delete them here
    //glDetachShader(program, vertexShader);
    //glDeleteShader(vertexShader);
    glDeleteProgram(program);
    loaded = false;
    valid = false;
}

/*
 * This function should set up which generic attribute attaches to which
 * input variable of the vertex shader. I always make my vertex shaders
 * use the same basic names (i.e "position" for positions...) so that it
 * becomes trivial to attach vertex data to shaders, without having to
 * mess around with layout qualifiers in the shaders themselves etc.
 */
void Shader::setDefaultAttributes()	{
    Renderer *renderer = Naquadah::getRenderer();
    if (renderer != nullptr) {
        bindAttributeLocation(program, VERTEX_BUFFER, "position");
        bindAttributeLocation(program, UV_MAP_BUFFER, "uv_map");
        bindAttributeLocation(program, NORMAL_BUFFER, "normal");
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

void Shader::updateShaderParameters(bool forceUpdate) {
    for (int i = 0; i < shaderParameters->size(); i++) {
        ShaderParameter *parameter = shaderParameters->at(i);
        if (parameter->hasValueChanged() || forceUpdate) {
            GLuint location = glGetUniformLocation(program, parameter->parameterName.c_str());
            if (location != -1) {
                switch (parameter->parameterType) {
                case PARAMETER_INT:
                    glUniform1iv(location, parameter->valueSize, (int*) parameter->getValue());
                    break;
                case PARAMETER_FLOAT:
                    glUniform1fv(location, parameter->valueSize, (float*) parameter->getValue());
                    break;
                case PARAMETER_DOUBLE:
                    glUniform1dv(location, parameter->valueSize, (double*) parameter->getValue());
                    break;
                case PARAMETER_VECTOR_2:
                    glUniform2fv(location, parameter->valueSize, (float*) parameter->getValue());
                    break;
                case PARAMETER_VECTOR_3:
                    glUniform3fv(location, parameter->valueSize, (float*) parameter->getValue());
                    break;
                case PARAMETER_VECTOR_4:
                    glUniform4fv(location, parameter->valueSize, (float*) parameter->getValue());
                    break;
                case PARAMETER_MATRIX_3:
                    glUniformMatrix3fv(location, parameter->valueSize, false, (float*) parameter->getValue());
                    break;
                case PARAMETER_MATRIX_4:
                    glUniformMatrix4fv(location, parameter->valueSize, false, (float*) parameter->getValue());
                    break;
                }
            }
        }
    }
}

void Shader::addShaderParameter(ShaderParameter *shaderParameter) {
    bool exists = false;
    for (int i = 0; i << shaderParameters->size(); i++) {
        if (shaderParameters->at(i)->parameterName == shaderParameter->parameterName) {
            exists = true;
        }
    }
    if (!exists) {
        shaderParameters->emplace_back(shaderParameter);
    }
}
    
void Shader::removeShaderParameter(std::string parameterName) {
    for (int i = 0; i << shaderParameters->size(); i++) {
        if (shaderParameters->at(i)->parameterName == parameterName) {
            ShaderParameter *shaderParameter = shaderParameters->at(i);
            shaderParameters->erase(shaderParameters->begin() + i);
            delete shaderParameter;
            return;
        }
    }
}

ShaderParameter *Shader::getShaderParameter(std::string parameterName) {
    for (int i = 0; i < shaderParameters->size(); i++) {
        if (shaderParameters->at(i)->parameterName == parameterName) {
            return shaderParameters->at(i);
        }
    }
    return nullptr;
}


Shader *Shader::getOrCreate(int name, const std::string &vertexFilename, const std::string &fragFilename,
    bool preLoad) {
    if (ResourcesManager::resourceExists(name)) {
        return (Shader*) ResourcesManager::getResource(name);
    } else {
        Shader *newShader = new Shader(name, vertexFilename, fragFilename);
        ResourcesManager::addResource(newShader, preLoad);
        return newShader;
    }
}

bool Shader::operator==(Shader &other) {
    if (&other)
        return this->program == other.program;
    return false;
}

bool Shader::operator!=(Shader &other) {
    return !(*this == other);
}