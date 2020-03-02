#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <string>

#include "../compat/opengl.h"


class GlTexture
{
private:
    GLuint handle;
    int width;
    int height;
public:
    GlTexture(int w, int h, unsigned char* data, GLenum type = GL_RGBA);
    GlTexture(std::string file);
    ~GlTexture();

    GLuint getHandle();
};

#endif // GLTEXTURE_H
