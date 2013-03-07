CC=gcc
CFLAGS=

CXX=g++
CXXFLAGS=

CPPFLAGS=$(CCOPTS) -I../stb 

LD=g++
LDFLAGS=
LDLIBS=-lGLES_CM -lopenal -lglfw3 -lX11 -lXrandr -lXxf86vm -lEGL

OBJS=../stb/stb_image.o ../stb/stb_vorbis.o ../main.o

all : $(OUT)

$(OUT) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)