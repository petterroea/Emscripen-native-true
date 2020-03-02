APPNAME=project
SOURCES := $(shell find $(SOURCEDIR) -name '*.cpp')

# Emscripten variables

CPPFLAGS=-std=c++14 -O2
EMSFLAGS=-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='["png"]' -s FULL_ES3=1 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1 --preload-file assets/

EM_COMPILER=emcc
SOURCEDIR=src

# Linux building stuff
ODIR=obj
OUTPUT=bin
CC=g++
LINUX_CPPFLAGS=-I/usr/include -g
LDFLAGS=-g
LIBS=-lSDL2 -lSDL2_image -lGL -lGLEW

OBJS=$(subst .cpp,.o,$(SOURCES))

.PHONY: all
all: emscripten/index.js $(OUTPUT)/$(APPNAME)


# Compile C++

$(OUTPUT):
	mkdir $@

$(OUTPUT)/$(APPNAME): src/**.cpp $(OUTPUT)
	$(CC) $(SOURCES) $(CPPFLAGS) $(LINUX_CPPFLAGS) $(LIBS) -o $@


# Compile emscripten

emscripten/index.js: src/**.cpp
	$(EM_COMPILER) $(SOURCES) $(CPPFLAGS) $(EMSFLAGS) -o index.js 

#tool: $(OBJS)
#	$(CC) $(LDFLAGS) -o tool $(OBJS) $(LIBS)

#%.o: src\%.cpp
#	$(CC) $(CPPFLAGS) $(LINUX_CPPFLAGS) -c $<

.PHONY: clean
clean:
	rm $(OUTPUT)/$(APPNAME)
	rm index.wasm
	rm index.js

.PHONY: serve
serve:
	python2 -m SimpleHTTPServer 8080
