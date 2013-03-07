
//Lang/OS...
#include <ctime>
#include <cmath>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <typeinfo>
#include <signal.h>

#if _WIN32
#include <windows.h>
#include <direct.h>
#include <sys/stat.h>
#undef LoadString

#elif __APPLE__
#include <mach-o/dyld.h>
#include <sys/stat.h>
#include <dirent.h>
#include <copyfile.h>

#elif __linux
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>
#endif

// Graphics/Audio stuff

//OpenGL...
#define GLFW_INCLUDE_ES1
#include <GL/glfw3.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

//OpenAL...
#include <AL/al.h>
#include <AL/alc.h>

//STB_image lib...
#define STBI_HEADER_FILE_ONLY
#include <stb_image.c>

//stb_vorbis lib
#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

#define glOrtho	  glOrthof
