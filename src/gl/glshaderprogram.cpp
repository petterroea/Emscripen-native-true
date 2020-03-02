#include "glshaderprogram.h"
#include "../logger.h"

GlShaderProgram::GlShaderProgram()
{
    handle = glCreateProgram();
}

GlShaderProgram::~GlShaderProgram()
{
    glDeleteProgram(handle);
}

void GlShaderProgram::attachShader(GlShader &shader)
{
   glAttachShader(handle, shader.getShaderHandle());
}

void GlShaderProgram::link()
{
    if(linked) {
        logMessage(ERROR, "link() called on already linked shader");
    }

    glLinkProgram(handle);
    GLint isLinked = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint length = 0;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);

        char* infoLog = new char[length];
        glGetProgramInfoLog(handle, length, &length, infoLog);

        logMessage(ERROR, "Failed to link shader program");
        logMessage(ERROR, infoLog);
    }
    linked = true;
}

GLint GlShaderProgram::getHandle() const
{
    return handle;
}
