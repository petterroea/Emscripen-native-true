#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       

#define GL_GLEXT_PROTOTYPES 1

#ifdef EMSCRIPTEN
#include "compat/opengl.h"
#else
#include <GL/glew.h>
bool game_running = true;
#endif

SDL_Window* window;


#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

//////////////////////////////////////////////////
// BEGIN custom includes
#include "logger.h"
#include "scene/glscene.h"
#include "scene/textureTestScene.h"
#include "scene/particleTestScene.h"
#include "scene/modelTestScene.h"

// an example of something we will control from the javascript side
//bool background_is_black = true;

// the function called by the javascript code
//extern "C" void EMSCRIPTEN_KEEPALIVE toggle_background_color() { background_is_black = !background_is_black; }

uint32_t lastDelta = 0;
GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};

GLScene* currentScene = NULL;

void main_loop() { 
    // move a vertex
    const uint32_t milliseconds_since_start = SDL_GetTicks();
    const uint32_t milliseconds_per_loop = 3000;
    //vertices[0] = ( milliseconds_since_start % milliseconds_per_loop ) / float(milliseconds_per_loop) - 0.5f;
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uint32_t delta = milliseconds_since_start-lastDelta;
    lastDelta = milliseconds_since_start;

    //Event handling

    SDL_Event event;
    /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
    /* more events on the event queue, our while loop will exit when */
    /* that occurs.                                                  */
    while( SDL_PollEvent( &event ) ){
        /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
        switch( event.type ){
            case SDL_KEYDOWN:
            printf( "Key press detected\n" );
            #ifndef EMSCRIPTEN
            if(event.key.keysym.sym == SDLK_ESCAPE){
                game_running = false;
            }
            #endif
            break;

            case SDL_KEYUP:
            printf( "Key release detected\n" );
            break;

            case SDL_QUIT:
#ifndef EMSCRIPTEN
            game_running = false;
#endif
            break;

            default:
            break;
        }
    }


    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    currentScene->render(delta);

    // Draw a triangle from the 3 vertices
    //glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(window);

    //Handle opengl errors
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        logMessage(ERROR, "OpenGL error: " + std::to_string(err));
    }
}

#ifdef EMSCRIPTEN
    bool init_emscripten() {
        SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, nullptr);
        return true;
    }
#else
    bool init_native() {
        if(SDL_Init(SDL_INIT_VIDEO) < 0) {
            logMessage(ERROR, "SDL_Init failed");
            return false;
        }

        window = SDL_CreateWindow( "SDL program", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

        if(window == NULL) {
            logMessage(ERROR, "Failed to create window");
            return false;
        }
    }
#endif

int main()
{
    srand(time(NULL));

    logMessage(INFO, "Hello");

#ifdef EMSCRIPTEN
    init_emscripten();
#else
    init_native();
#endif

    logMessage(INFO, "SDL initialized");

    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#ifdef EMSCRIPTEN
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
#endif
    SDL_GLContext context = SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(1);


    logMessage(INFO, "Initializing image");

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
       logMessage(ERROR, std::string("SDL_image could not initialize! SDL_image Error: ") + IMG_GetError() );
    }

    //Set up glew
#ifndef EMSCRIPTEN
    glewExperimental = GL_TRUE;
    glewInit();
#else
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);  
    glEnable(GL_DEPTH_TEST);

    logMessage(INFO, "Sucessfully started the application");

    //Initialize a scene
    currentScene = new ModelTestScene();

#ifdef EMSCRIPTEN
    emscripten_set_main_loop(main_loop, 0, true);
#else
    while(game_running) {
        main_loop();
    }
    delete currentScene;
#endif
    return EXIT_SUCCESS;
}