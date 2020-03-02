#ifndef GLBUFFEROBJECT_H
#define GLBUFFEROBJECT_H

#include "../compat/opengl.h"


class GlMutableBufferObject
{
private:
    GLuint handle;
    GLenum target;
public:
    GlMutableBufferObject(GLenum target);
    ~GlMutableBufferObject();
    void bind();
    void upload(const char* data, int length, GLenum usage = GL_STATIC_DRAW);
    inline void uploadFloats(const float* data, int length, GLenum usage = GL_STATIC_DRAW) {upload( (char*)data, length*sizeof(float), usage);};
    GLuint getHandle();
};

#endif // GLBUFFEROBJECT_H
