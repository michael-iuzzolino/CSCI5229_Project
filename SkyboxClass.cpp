// SkyboxClass.cpp
#include "SkyboxClass.h"

SkyboxClass::SkyboxClass(float newWorldDimension)
{
    worldDimension = newWorldDimension;
    skyboxIDX = 0;
}

void SkyboxClass::updateAzimuthRotation(float angle)
{
    skyBoxAzimuth = angle;
}

void SkyboxClass::clampTexture()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void SkyboxClass::renderVAOApproach()
{
    glPushMatrix();

    GLuint shaderID = shaderProgramIDs[0];
    glUseProgram(shaderID);

    GLuint skyboxID = glGetUniformLocation(shaderID, "skybox");
    glUniform1i(skyboxID, 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
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
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default

    glPopMatrix();
}

void SkyboxClass::render()
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
    glPushMatrix();

    glRotatef(-skyBoxAzimuth, 0, 1, 0);
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);

    //  Sides
    glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
    clampTexture();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-worldDimension, -worldDimension, -worldDimension);
    glTexCoord2f(1.0, 0.0); glVertex3f(+worldDimension, -worldDimension, -worldDimension);
    glTexCoord2f(1.0, 1.0); glVertex3f(+worldDimension, +worldDimension, -worldDimension);
    glTexCoord2f(0.0, 1.0); glVertex3f(-worldDimension, +worldDimension, -worldDimension);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
    clampTexture();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(+worldDimension, -worldDimension, -worldDimension);
    glTexCoord2f(1.0, 0.0); glVertex3f(+worldDimension, -worldDimension, +worldDimension);
    glTexCoord2f(1.0, 1.0); glVertex3f(+worldDimension, +worldDimension, +worldDimension);
    glTexCoord2f(0.0, 1.0); glVertex3f(+worldDimension, +worldDimension, -worldDimension);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDs[2]);
    clampTexture();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(+worldDimension, -worldDimension, +worldDimension);
    glTexCoord2f(1.0, 0.0); glVertex3f(-worldDimension, -worldDimension, +worldDimension);
    glTexCoord2f(1.0, 1.0); glVertex3f(-worldDimension, +worldDimension, +worldDimension);
    glTexCoord2f(0.0, 1.0); glVertex3f(+worldDimension, +worldDimension, +worldDimension);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDs[3]);
    clampTexture();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-worldDimension, -worldDimension, +worldDimension);
    glTexCoord2f(1.0, 0.0); glVertex3f(-worldDimension, -worldDimension, -worldDimension);
    glTexCoord2f(1.0, 1.0); glVertex3f(-worldDimension, +worldDimension, -worldDimension);
    glTexCoord2f(0.0, 1.0); glVertex3f(-worldDimension, +worldDimension, +worldDimension);
    glEnd();

    //  Top and bottom
    glBindTexture(GL_TEXTURE_2D, textureIDs[4]);
    clampTexture();

    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 1.0); glVertex3f(+worldDimension, +worldDimension, -worldDimension);
    glTexCoord2f(0.0, 1.0); glVertex3f(+worldDimension, +worldDimension, +worldDimension);
    glTexCoord2f(0.0, 0.0); glVertex3f(-worldDimension, +worldDimension, +worldDimension);
    glTexCoord2f(1.0, 0.0); glVertex3f(-worldDimension, +worldDimension, -worldDimension);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureIDs[5]);
    clampTexture();

    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 0.0); glVertex3f(-worldDimension, -worldDimension, +worldDimension);
    glTexCoord2f(1.0, 1.0); glVertex3f(+worldDimension, -worldDimension, +worldDimension);
    glTexCoord2f(0.0, 1.0); glVertex3f(+worldDimension, -worldDimension, -worldDimension);
    glTexCoord2f(0.0, 0.0); glVertex3f(-worldDimension, -worldDimension, -worldDimension);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
    glDepthMask(1);
    glEnable(GL_DEPTH_TEST);
}

void SkyboxClass::setTextureID(GLuint newSkyboxID)
{
    skyboxTextureID = newSkyboxID;
}
