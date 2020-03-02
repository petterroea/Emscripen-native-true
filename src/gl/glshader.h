#ifndef GLSHADER_H
#define GLSHADER_H

#include <string>
#include "../compat/opengl.h"

class GlShader
{
    GLint handle;
    GLenum shaderType;
public:
    GlShader(std::string filename, GLenum shaderType);
    ~GlShader();
    int getShaderHandle() const;
    GLenum getShaderType() const;
};

#endif // GLSHADER_H
