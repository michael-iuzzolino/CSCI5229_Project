# main
EXE=main

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -msse3 -DUSEGLEW
LIBS=-lglew32 -lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations -msse3
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall -msse3
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
main.o: main.cpp projectManager.h CameraClass.h LightClass.h TerrainClass.h ShaderHandlerClass.h SkyboxClass.h glm/glm.hpp
CameraClass.o: CameraClass.cpp CameraClass.h
LightClass.o: LightClass.cpp LightClass.h
TerrainClass.o: TerrainClass.cpp TerrainClass.h
ShaderHandlerClass.o: ShaderHandlerClass.cpp ShaderHandlerClass.h
SkyboxClass.o: SkyboxClass.cpp SkyboxClass.h
fatal.o: fatal.c projectManager.h
loadtexbmp.o: loadtexbmp.c projectManager.h
object.o: object.c projectManager.h
print.o: print.c projectManager.h
errcheck.o: errcheck.c projectManager.h

# Create archive
projectManager.a: fatal.o loadtexbmp.o object.o print.o errcheck.o print.o ShaderHandlerClass.o SkyboxClass.o CameraClass.o LightClass.o TerrainClass.o
	ar -rcs $@ $^
# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -c -Wno-narrowing $(CFLG)  $<

#  Link
main:main.o projectManager.a
	gcc -O3 -o $@ $^   $(LIBS) -lstdc++

#  Clean
clean:
	$(CLEAN)
