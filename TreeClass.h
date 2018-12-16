// TreeClass.h
#ifndef _TreeClass_H
#define _TreeClass_H
#include "projectManager.h"

class TreeClass
{
public:
    int MAX_BRANCH_DEPTH;
    float TREE_FACTOR;
    float TREE_LENGTH_BASE;

    float TREE_RADIUS_BASE;
    float TREE_RADIUS_FACTOR;

    vector<GLuint> textureIDs;
    vector<GLuint> shaderProgramIDs;

    bool texturesOn;
    bool showLeaves;

    float lengthResolution;

    TreeClass();
    void initializeTextureIDs(vector<GLuint> ids);
    void initializeShaderIDs(vector<GLuint> ids);
    void render(int i, int j, glm::vec3 v1, float);
private:
    float windRotation;
    void setupTextures();
    void drawBranch(int currentDepth, float deg);
    void renderLeaf(int leaf_i, float z0, float scale);
    void createCylinder(int nmax, int nmin, float height, float radius, int currentDepth);
    void reflectBranch(float length, float radius, int currentDepth, float deg);
};

#endif
