// SkyboxClass.h
#ifndef _SkyboxClass_H
#define _SkyboxClass_H
#include "projectManager.h"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class SkyboxClass
{
public:

    float worldDimension;
    float skyBoxAzimuth;
    vector<string> textureFiles = {
        "skybox/01/front.bmp",  "RGB",
        "skybox/01/left.bmp",   "RGB",
        "skybox/01/back.bmp",   "RGB",
        "skybox/01/right.bmp",  "RGB",
        "skybox/01/top.bmp",    "RGB",
        "skybox/01/bottom.bmp", "RGB"
    };

    vector<GLuint> textureIDs;
    GLuint skyboxTextureID;

    vector<string> shaderFiles = {
        "skybox/skybox.vert", "skybox/skybox.frag"
    };
    vector<GLuint> shaderProgramIDs;

    int skyboxIDX;

    SkyboxClass(float);
    void render();
    void updateAzimuthRotation(float);
    void setTextureID(GLuint);

private:
    void renderSide(GLuint, float, float, float, float, float, float);
    void clampTexture();
    void renderVAOApproach();
};
#endif
