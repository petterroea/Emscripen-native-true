#include <SDL2/SDL_image.h>
#include <string>

#include "gltexture.h"
#include "../logger.h"

GlTexture::GlTexture(int w, int h, unsigned char* data, GLenum type) : width(w), height(h)
{
    /* Create The Texture */
    glGenTextures( 1, &this->handle );

    /* Typical Texture Generation Using Data From The Bitmap */
    glBindTexture( GL_TEXTURE_2D, this->handle );

    /* Generate The Texture */
    glTexImage2D( GL_TEXTURE_2D, 0, 3, w,
      h, 0, type,
      GL_UNSIGNED_BYTE, data );
    glGetError();

    /* Linear Filtering */
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

GlTexture::GlTexture(std::string file)
{
    logMessage(INFO, "Loading texture " + file);
    /* Create storage space for the texture */
    SDL_Surface *TextureImage[1];

    /* Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit */
    if ( ( TextureImage[0] = IMG_Load(file.c_str()) ) )
    {

        /* Create The Texture */
        glGenTextures( 1, &this->handle );

        /* Typical Texture Generation Using Data From The Bitmap */
        glBindTexture( GL_TEXTURE_2D, this->handle );

        /* Generate The Texture */
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, TextureImage[0]->w,
          TextureImage[0]->h, 0, GL_RGBA,
          GL_UNSIGNED_BYTE, TextureImage[0]->pixels );
        glGetError();

        /* Linear Filtering */
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        logMessage(INFO, "Successfully loaded texture for handle " + std::to_string(this->handle));
    } else {
        logMessage(WARNING, std::string("Failed to load texture ") + file);
    }

    /* Free up any memory we may have used */
    if ( TextureImage[0] )
        SDL_FreeSurface( TextureImage[0] );
}

GlTexture::~GlTexture()
{
    glDeleteTextures(1, &this->handle);
}

GLuint GlTexture::getHandle()
{
    return this->handle;
}
