#include "glmutablebufferobject.h"


GlMutableBufferObject::GlMutableBufferObject(GLenum target)
{
    this->target = target;
    glGenBuffers(1, &this->handle);
}

GlMutableBufferObject::~GlMutableBufferObject()
{
    glDeleteBuffers(1, &this->handle);
}

void GlMutableBufferObject::bind()
{
    glBindBuffer(target, handle);
}

void GlMutableBufferObject::upload(const char *data, int length, GLenum usage)
{
    bind();
    glBufferData(target, length, data, usage);

}

GLuint GlMutableBufferObject::getHandle()
{
    return this->handle;
}
