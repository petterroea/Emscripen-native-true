// OpenGL include multiplexing 

#ifdef EMSCRIPTEN

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

//OpenGL stuff
#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES

#define glVertexAttribDivisor glVertexAttribDivisorNV
#define glDrawArraysInstanced glDrawArraysInstancedNV

#else

#include <GL/glew.h>


#endif