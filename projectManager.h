// Code derived from CSCI 5229 course content by Willem A. (Vlakkies) Schreüder

#ifndef projectManager
#define projectManager
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MAXLEN 8192  //  Maximum length of text string
#define M_PI 3.14159265358979323846
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

void Fatal(const char* format , ...);

#ifdef __cplusplus
}
#endif

#include <string>
#include <fstream>
#include <istream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <iterator>
#include <random>
#include <ctime>

#include "Dependencies/glm/vec3.hpp"
#include "Dependencies/glm/vec4.hpp"
#include "Dependencies/glm/mat4x4.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include "Dependencies/PerlinNoise.hpp"

#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;

#endif
