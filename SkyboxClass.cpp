// SkyboxClass.cpp
#include "SkyboxClass.h"

SkyboxClass::SkyboxClass(float newWorldDimension)
{
    worldDimension = newWorldDimension;
    show = true;
}

void SkyboxClass::initCubeMap()
{
    // Code derived from https://learnopengl.com/Advanced-OpenGL/Cubemaps
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void SkyboxClass::render()
{
    if (!show)
    {
        return;
    }

    glDepthFunc(GL_LEQUAL);
    glPushMatrix();

    glUseProgram(shaderProgramIDs[0]);

    glRotatef(-skyBoxAzimuth, 0, 1, 0);
    glRotatef(180, 1, 0, 0);
    glScalef(worldDimension, worldDimension, worldDimension);
    glColor3f(1, 1, 1);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glUseProgram(0);

    glPopMatrix();
    glDepthMask(1);
    glDepthFunc(GL_LESS);
}

GLuint SkyboxClass::getCubemapTexture()
{
    return cubemapTextureID;
}

float SkyboxClass::getSkyboxAzimuth()
{
    return skyBoxAzimuth;
}

void SkyboxClass::toggleShow()
{
    show = !show;
}

void SkyboxClass::updateAzimuthRotation(float angle)
{
    skyBoxAzimuth = angle;
}
