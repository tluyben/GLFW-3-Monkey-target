GLFW-3-Monkey-target
====================

Monkey target GLFW 3 (for OpenPandora & Rasberry PI)



The current version of Monkeycoder.co.nz does not work on Pandora or Rasberry PI. Unfortunately those are 
my primary targets so I tried to resolve this and succeeded (kind of). 

Installation (on x86 Mac, Linux or Windows to bootstrap the ARM versions): 

- git clone git@github.com:tluyben/GLFW-3-Monkey-target.git
- rsync -r GLFW-3-Monkey-target/* /PATH/TO/YOUR/MonkeyPro68/
- open up src/transcc/builders/builders.monkey and add

	builders.Set "glfw3", New GlfwBuilder( tcc )
- because I cannot copy Mojo (it's not freeware like the rest of Monkey); copy modules/mojo/native/mojo.glfw.cpp to
modules/mojo/native/mojo.glfw3.cpp 
- open modules/mojo/native/mojo.glfw3.cpp in an editor and search for glfwGetWindowSize, change to glfwGetWindowSize( _window, &width,&height ); (there are two) 
- do the same for glfwSwapBuffers -> glfwSwapBuffers(_window); and save the file
- open src/transcc/transcc.monkey and compile with C++ Tool
- copy the entire Monkey install over to the Pandora or Rasberry (or other) 
- cd src/transcc/transcc.build/cpptool; g++ -o ../../../../bin/transcc_linux_arm main.cpp 
- now you have a working compiler for cpptool projects on your target system 

Now, the following things are working/tested on Pandora currently only, but for someone with 
Linux experience they should not be too hard to fix for any other platform. 

All the below should be executed on the target device (Pandora / Rasberry / ...)

- install the linux dev tools; for OpenPandora/Angstrom you need the latest cdevtools PND, for 
Debian you need build-essentials 
- install or check if you have xf86vidmode installed, for Angstrom this is 

opkg install libxxf86vm1
opkg install libxxf86vm-dev

or, in the new cdevtools to not impurify your system; 

addipk libxxf86vm1
addipk libxxf86vm-dev

- Download http://connect.creativelabs.com/openal/default.aspx, compile and install (very 
straight forward! wish all compiles in the world were like this)

- Download https://github.com/elmindreda/glfw 
- Open CMakeLists.txt
- add set(GLFW_USE_EGL, 1) on top 
- find GLFW_CLIENT_LIBRARY and set it to glesv1 

- There is an issue with getting the GL version under OpenPandora; I'm hoping someone will help with that, but for now
I hardcoded it (version = (const char*) glGetString(GL_VERSION); returns Null...) ; open de file in the GLFW3 source tree;
src/context.c and change the top function to; https://gist.github.com/tluyben/5110198

- run cmake .; make ; make install 



- check if all is ok and the following files exist

* /usr/lib/libopenal.so.1 
* /usr/include/AL/*.h
* /usr/lib/libglfw3.a
* /usr/include/GL/*.h

if the files do not exist, symlink them, change the include and library paths OR redo the compile/installs with different directories (they 
were fine for me right away) 

Now we should be done and are ready to test, my favorite: 

- cd bananas/skn3/monkenstein
- ../../../bin/transcc_linux_arm -target=Glfw3_Game monkenstein.monkey

which should generate the build directory and everything you need and build the software and run it. 

If anything goes wrong, go to monkenstein.build/glfw/gcc_linux and run make -OUT=Debug/MonkeyGame by hand; see what 
the errors are , you can send them to me or solve yourself; please hit me up if you do. 

Note: all commands / copies are relative to the MonkeyPro68 (or higher) main directory.

TODO: 

- test everything building wise and smooth it out
- test sound (didn't do that at all yet) 
- simplify/automate setup
- make Pandora PND 
- test on Rasberry PI 
