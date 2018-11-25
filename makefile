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
main.o: main.cpp projectManager.h CameraClass.h LightClass.h WaterClass.h TerrainClass.h ShaderHandlerClass.h SkyboxClass.h TextureHandlerClass.h glm/glm.hpp
CameraClass.o: CameraClass.cpp CameraClass.h
LightClass.o: LightClass.cpp LightClass.h
TerrainClass.o: TerrainClass.cpp TerrainClass.h
WaterClass.o: WaterClass.cpp WaterClass.h
stb_image.o: stb_image.cpp stb_image.h
stb_image.o: stb_image_write.cpp stb_image_write.h
TextureHandlerClass.o: TextureHandlerClass.cpp TextureHandlerClass.h
ShaderHandlerClass.o: ShaderHandlerClass.cpp ShaderHandlerClass.h
SkyboxClass.o: SkyboxClass.cpp SkyboxClass.h
fatal.o: fatal.cpp projectManager.h

# Create archive
projectManager.a: fatal.o stb_image.o stb_image_write.o WaterClass.o ShaderHandlerClass.o SkyboxClass.o CameraClass.o LightClass.o TerrainClass.o TextureHandlerClass.o
	ar -rcs $@ $^
# Compile rules
.cpp.o:
	g++ -c -Wno-narrowing $(CFLG)  $<

#  Link
main:main.o projectManager.a
	g++ -O3 -o $@ $^   $(LIBS) -lstdc++

#  Clean
clean:
	$(CLEAN)
