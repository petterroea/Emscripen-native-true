#include <fstream>
#include "glshader.h"
#include "../logger.h"

GlShader::GlShader(std::string source, GLenum shaderType)
{
    handle = glCreateShader(shaderType);
    this->shaderType = shaderType;
    if(handle == 0) {
        logMessage(ERROR, "Shader creation error");
    }
    const char* str = source.c_str();
    const int length = source.length();

    glShaderSource(handle, 1, &str, &length);
    glCompileShader(handle);

    int result = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE) {
        logMessage(ERROR, "Shader compilation error");
        int logSize = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);

        char* errorlog = new char[logSize];

        glGetShaderInfoLog(handle, logSize, &logSize, errorlog);

        logMessage(ERROR, errorlog);

        delete[] errorlog;
    }
}

GlShader::~GlShader()
{
    glDeleteShader(handle);
}

int GlShader::getShaderHandle() const
{
    return this->handle;
}

GLenum GlShader::getShaderType() const
{
    return this->shaderType;
}

