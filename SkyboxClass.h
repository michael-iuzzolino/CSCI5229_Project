// SkyboxClass.h
#ifndef _SkyboxClass_H
#define _SkyboxClass_H
#include "projectManager.h"

class SkyboxClass
{
public:
    vector<string> textureFiles = {
        "textures/skybox/01/left.tga",
        "textures/skybox/01/right.tga",
        "textures/skybox/01/bottom.tga",
        "textures/skybox/01/top.tga",
        "textures/skybox/01/front.tga",
        "textures/skybox/01/back.tga"
    };
    GLuint cubemapTextureID;

    vector<string> shaderFiles = {
        "skybox/skybox.vert", "skybox/skybox.frag"
    };
    vector<GLuint> shaderProgramIDs;

    SkyboxClass(float);
    void render();
    void updateAzimuthRotation(float);
    void toggleShow();

    void initCubeMap();
    GLuint getCubemapTexture();
    float getSkyboxAzimuth();
private:
    bool show;
    float worldDimension;
    float skyBoxAzimuth;
    GLuint skyboxVAO, skyboxVBO;
    void renderVAOApproach();
};
#endif
