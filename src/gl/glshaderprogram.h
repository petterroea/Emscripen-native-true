#ifndef GL_SHADER_PROGRAM_H
#define GL_SHADER_PROGRAM_H

#include "../compat/opengl.h"
#include "glshader.h"

class GlShaderProgram
{
private:
    GLint handle;
    bool linked = false;
public:
    GlShaderProgram();
    ~GlShaderProgram();
    void attachShader(GlShader &shader);
    void link();
    GLint getHandle() const;
};

#endif // GL_SHADER_PROGRAM_H
