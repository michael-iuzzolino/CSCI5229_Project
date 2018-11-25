// WaterClass.h
#ifndef _WaterClass_H
#define _WaterClass_H
#include "projectManager.h"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <fstream>
#include <istream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class WaterClass
{
public:

    float waterHeight;
    int wavesOn;
    int currentShaderID;
    int currentTextureID;
    int showNormals;

    vector<string> shaderFiles = {
        "water/waves.vert", "water/waves.frag"
    };
    vector<GLuint> shaderProgramIDs;

    vector<string> textureFiles = {
        "water/water.bmp",  "RGB",
        "water/water2.bmp", "RGB"
    };
    vector<GLuint> textureIDs;

    WaterClass();
    void initialize();
    void render();
    string getShaderText();
    void toggleShader();
    void toggleShowNormals();

private:
    void cleanUpRender();
    void setShaders();
    void setTextures();
    void setMaterials();
    void renderNormals();
};

#endif
