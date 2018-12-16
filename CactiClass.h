// CactiClass.h
#ifndef _CactiClass_H
#define _CactiClass_H
#include "projectManager.h"

class CactiClass
{
public:

    vector<GLuint> textureIDs;
    vector<GLuint> shaderProgramIDs;

    bool texturesOn;
    bool showLeaves;

    float lengthResolution;

    CactiClass();
    void initializeTextureIDs(vector<GLuint> ids);
    void initializeShaderIDs(vector<GLuint> ids);
    void render(int i, int j, glm::vec3 v1, float);
private:
    float windRotation;
    void setupTextures();
    void drawBranch(int, float);
    void Vertex(double, double);
    void createCylinder(int, int, float, float, bool);
};

#endif
