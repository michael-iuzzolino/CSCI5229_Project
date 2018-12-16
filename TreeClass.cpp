// TreeClass.cpp
#include "TreeClass.h"

TreeClass::TreeClass()
{
    MAX_BRANCH_DEPTH = 3;
    TREE_FACTOR = 0.75;
    TREE_LENGTH_BASE = 2;
    TREE_RADIUS_BASE = 0.2;
    TREE_RADIUS_FACTOR = 0.4;

    lengthResolution = 10;
    windRotation = 0;

    texturesOn = true;
    showLeaves = true;
}

void TreeClass::initializeTextureIDs(vector<GLuint> ids)
{
    textureIDs.clear();
    for (int i=0; i < (int)ids.size(); i++)
    {
        cout << ids[i] << endl;
        textureIDs.push_back(ids[i]);
    }
}

void TreeClass::initializeShaderIDs(vector<GLuint> ids)
{
    shaderProgramIDs.clear();
    for (int i=0; i < (int)ids.size(); i++)
    {
        cout << ids[i] << endl;
        shaderProgramIDs.push_back(ids[i]);
    }
}

void TreeClass::renderLeaf(int leaf_i, float z0, float scale)
{
    if (texturesOn)
    {
        glUseProgram(shaderProgramIDs[0]);
    }
    float zTrans = (leaf_i % 3 == 0) ? 0.05 : -0.05;
    float xTrans = (leaf_i % 2 == 0) ? 0.05 : -0.05;
    glPushMatrix();
        glTranslatef(xTrans, z0, zTrans);
        glScalef(scale, scale, scale);
        glRotatef(windRotation, 1, 0, 0);
        glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 0); glVertex3f(-1, 0, -1);
        glTexCoord2f(1, 0); glVertex3f(1, 0, -1);
        glTexCoord2f(1, 1); glVertex3f(1, 0, 1);
        glTexCoord2f(0, 1); glVertex3f(-1, 0, 1);
        glEnd();
    glPopMatrix();
    if (texturesOn)
    {
        glUseProgram(0);
    }
}

void TreeClass::createCylinder(int nmax, int nmin, float height, float radius, int currentDepth)
{
    float leafScale = 0.5;

    if (texturesOn)
    {
        glEnable(GL_TEXTURE_2D);
        glUseProgram(0);
    }
    if (currentDepth <= MAX_BRANCH_DEPTH-2)
    {
        // glColor3f(0.62745098, 0.32156863, 0.17647059);
    }
    else
    {
        // glColor3f(0.87058824, 0.72156863, 0.52941176);
    }
    bool leavesOn = (showLeaves) ? ((currentDepth <= MAX_BRANCH_DEPTH-2) ? 0 : 1) : 0;

    double MAX = height / nmax;
    double MIN = 2.0 * M_PI / nmin;
    int i, j;

    float originalRadius = radius;

    glPushMatrix();
        glEnable(GL_CULL_FACE);
        int branchLeafIDX = 0;
        GLfloat z0=1, z1=1;
        for (i=0; i < nmax; ++i)
        {
            z0 = 0.5 * height - i * MAX;
            z1 = z0 - MAX;

            glBegin(GL_TRIANGLE_STRIP);
            for (j=0; j <= nmin; ++j)
            {
                double a = j * MIN;
                GLfloat x = radius * cos(a);
                GLfloat y = radius * sin(a);
                glNormal3f(y / radius, 0.0, x / radius);
                glTexCoord2f(j / (GLfloat) nmin, i / (GLfloat) nmax);
                glVertex3f(y, z0, x);

                glNormal3f(y / radius, 0.0, x / radius);
                glTexCoord2f(j / (GLfloat) nmin, (i+1) / (GLfloat) nmax);
                glVertex3f(y, z1, x);
            }
            radius += nmax / 100000.;
            glTranslatef(0, -0.5, 0); // Narrow the branch
            glEnd();

            if (leavesOn && i % 10 == 0)
            {
                glPushMatrix();
                    renderLeaf(branchLeafIDX, z0, leafScale);
                glPopMatrix();
            }
            branchLeafIDX++;
        }

        glDisable(GL_CULL_FACE);

        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0, 0, 1);
        z0 = 0.5 * height - 0 * MAX;
        for (int k=0;k<=360;k+=10)
        {
            glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
            glVertex3f(originalRadius*Cos(k), z0, originalRadius*Sin(k));
        }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0, 0, -1);
        for (int k=0;k<=360;k+=10)
        {
            glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
            glVertex3f(originalRadius*Cos(k), z1, originalRadius*Sin(k));
        }
        glEnd();
    glPopMatrix();

    if (texturesOn)
    {
        glDisable(GL_TEXTURE_2D);
    }
}

void TreeClass::reflectBranch(float length, float radius, int currentDepth, float deg)
{
    glPushMatrix();
        glTranslatef(-length/4, length-length/4 - radius*2/3, 0);
        glRotatef(deg, 0, 0, 1);
        drawBranch(currentDepth+1, deg);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(length/4, length-length/4 - radius*2/3, 0);
        glRotatef(-deg, 0, 0, 1);
        drawBranch(currentDepth+1, deg);
    glPopMatrix();
}

void TreeClass::drawBranch(int currentDepth, float deg)
{
    if (currentDepth > MAX_BRANCH_DEPTH)
    {
        return;
    }

    if (currentDepth <= MAX_BRANCH_DEPTH - 2)
    {
        // glColor3d(0.45, 0.2, 0.05);
    }
    else
    {
        // glColor3d(0, 0.5, 0);
    }

    float length = TREE_LENGTH_BASE * pow(TREE_FACTOR, currentDepth);
    float radius = TREE_RADIUS_BASE * pow(TREE_RADIUS_FACTOR, currentDepth);

    glColor3f(length, length, length);

    createCylinder(lengthResolution, 10, length, radius, currentDepth);

    glPushMatrix();
        reflectBranch(length, radius, currentDepth, deg);
    glPopMatrix();

    glPushMatrix();
        glRotatef(67, 0, 1, 0);
        reflectBranch(length, radius, currentDepth, deg);
    glPopMatrix();
}

void TreeClass::setupTextures()
{
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLuint shaderID = shaderProgramIDs[0];
    glUseProgram(shaderID);

    GLuint leaf_ID_1 = glGetUniformLocation(shaderID, "texture1");
    GLuint leaf_ID_2 = glGetUniformLocation(shaderID, "texture2");
    GLuint leaf_ID_3 = glGetUniformLocation(shaderID, "texture3");

    float time = 0.001*glutGet(GLUT_ELAPSED_TIME);
    GLuint timeID = glGetUniformLocation(shaderID, "time");
    glUniform1f(timeID, time);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIDs[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureIDs[1]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureIDs[2]);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, textureIDs[3]);

    glUniform1i(leaf_ID_1, 1);
    glUniform1i(leaf_ID_2, 2);
    glUniform1i(leaf_ID_3, 3);

    glActiveTexture(GL_TEXTURE0);
}

void TreeClass::render(int i, int j, glm::vec3 v1, float updatedWindRotation)
{
    windRotation = updatedWindRotation;
    setupTextures();

    float deg = 37;
    glPushMatrix();
    glTranslatef(v1.x, v1.y, v1.z);
    glColor3f(1, 0, 0);
    drawBranch(0, deg);
    glPopMatrix();
}
