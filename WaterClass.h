// WaterClass.h
#ifndef _WaterClass_H
#define _WaterClass_H
#include "projectManager.h"

class WaterClass
{
public:

    float waterHeight;
    int wavesOn;
    int currentShaderID;
    int currentTextureID;

    GLuint skyboxTextureID;

    bool showNormals;
    bool showWater;
    bool reflectionON;

    float worldRotationRate;

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
    void toggleShow();
    void toggleWaterReflection();
    void setSkyboxTextureID(GLuint);
    void setSkyboxRotationRate(float);

private:
    void cleanUpRender();
    void setShaders();
    void setTextures();
    void setMaterials();
    void renderNormals();
};

#endif
