// WaterClass.cpp
#include "WaterClass.h"

WaterClass::WaterClass() {
    waterHeight = -8.5;
    wavesOn = 1;
    currentShaderID = 0;
    currentTextureID = 0;
}

void WaterClass::initialize() {}

void WaterClass::cleanUpRender()
{
    glDisable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
}

void WaterClass::setShaders()
{
    float time = 0.001*glutGet(GLUT_ELAPSED_TIME);

    GLuint shaderID = shaderProgramIDs[currentShaderID];
    glUseProgram(shaderID);

    GLuint timeUniformID = glGetUniformLocation(shaderID, "time");
    glUniform1f(timeUniformID, time);

    GLuint texNumUniformID = glGetUniformLocation(shaderID, "textureNum");
    glUniform1i(texNumUniformID, currentTextureID);

    GLuint textureUniformID_0 = glGetUniformLocation(shaderID, "texture0");
    GLuint textureUniformID_1 = glGetUniformLocation(shaderID, "texture1");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIDs[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureIDs[1]);

    glUniform1i(textureUniformID_0, 0);
    glUniform1i(textureUniformID_1, 1);
}

void WaterClass::setTextures()
{
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Blending
    glEnable(GL_BLEND);
    if (currentShaderID == 1)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    else
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
}

void WaterClass::setMaterials()
{
    float shinynessValue, emissionValue, specularLight, R, G, B, A;

    //  Set materials
    shinynessValue = 16;
    emissionValue = 0.0;
    specularLight = 0.0;
    R = 1.0;
    G = 1.0;
    B = 1.0;
    A = 1.0;

    float Shinyness[] = {shinynessValue};
    float RGBA[] = {R, G, B, A};
    float Specular[]  = {specularLight, specularLight, specularLight, 1.0};
    float Emission[]  = {emissionValue, emissionValue, emissionValue, 1.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Shinyness);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, RGBA);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RGBA);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
}

void WaterClass::renderNormals() {}

void WaterClass::render()
{
    int spatialScale = 512;

    glPushMatrix();
    setTextures();
    setMaterials();
    setShaders();
    glTranslatef(0, waterHeight, 0);
    glScalef(spatialScale, 1, spatialScale);

    glColor3f(0.75,0.75,0.75);
    glBegin(GL_QUADS);
    glm::vec3 v1(+1, 0, +1);
    glm::vec3 v2(+1, 0, -1);
    glm::vec3 v3(-1, 0, -1);
    glm::vec3 v4(-1, 0, +1);
    glm::vec3 normal = glm::normalize(glm::cross(v2-v1, v4-v1));
    glNormal3f(normal.x, normal.y, normal.z);
    float waterTextureRepeats = 100.0;

    // Vertex 1
    for (int i=0; i < (int)textureFiles.size(); i++)
    {
        glMultiTexCoord2f(GL_TEXTURE0 + i, 0.0, 0.0);
    }
    glVertex3f(v1.x, v1.y, v1.z);

    // Vertex 2
    for (int i=0; i < (int)textureFiles.size(); i++)
    {
        glMultiTexCoord2f(GL_TEXTURE0 + i, 0.0, waterTextureRepeats);
    }
    glVertex3f(v2.x, v2.y, v2.z);

    // Vertex 3
    for (int i=0; i < (int)textureFiles.size(); i++)
    {
        glMultiTexCoord2f(GL_TEXTURE0 + i, waterTextureRepeats, waterTextureRepeats);
    }
    glVertex3f(v3.x, v3.y, v3.z);

    // Vertex 4
    for (int i=0; i < (int)textureFiles.size(); i++)
    {
        glMultiTexCoord2f(GL_TEXTURE0 + i, waterTextureRepeats, 0.0);
    }
    glVertex3f(v4.x, v4.y, v4.z);

    glEnd();
    glPopMatrix();
    cleanUpRender();
}


string WaterClass::getShaderText()
{
    return shaderFiles[currentShaderID];
}

void WaterClass::toggleShader()
{
    currentTextureID = 1-currentTextureID;
}

void WaterClass::toggleShowNormals()
{
    showNormals = 1-showNormals;
}
