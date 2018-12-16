// CactiClass.cpp
#include "CactiClass.h"

CactiClass::CactiClass()
{
    lengthResolution = 10;
    windRotation = 0;

    texturesOn = true;
    showLeaves = true;
}

void CactiClass::initializeTextureIDs(vector<GLuint> ids)
{
    textureIDs.clear();
    for (int i=0; i < (int)ids.size(); i++)
    {
        cout << ids[i] << endl;
        textureIDs.push_back(ids[i]);
    }
}

void CactiClass::initializeShaderIDs(vector<GLuint> ids)
{
    shaderProgramIDs.clear();
    for (int i=0; i < (int)ids.size(); i++)
    {
        cout << ids[i] << endl;
        shaderProgramIDs.push_back(ids[i]);
    }
}

void CactiClass::Vertex(double th,double ph)
{
    // Function written by Willem A. (Vlakkies) Schreüder
    double x = Sin(th)*Cos(ph);
    double y = Cos(th)*Cos(ph);
    double z =         Sin(ph);
    //  For a sphere at the origin, the position
    //  and normal vectors are the same
    glNormal3d(x,y,z);
    glVertex3d(x,y,z);
}

void CactiClass::createCylinder(int nmax, int nmin, float height, float radius, bool capON)
{
    // Pieces of function derived from Willem A. (Vlakkies) Schreüder's CSCI 5229 Code
    double MAX = height / nmax;
    double MIN = 2.0 * M_PI / nmin;
    int i, j;
    int numRepeats = 5;
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
                glTexCoord2f(j / (GLfloat) nmin * numRepeats, i / (GLfloat) nmax * numRepeats);
                glVertex3f(y, z0, x);

                glNormal3f(y / radius, 0.0, x / radius);
                glTexCoord2f(j / (GLfloat) nmin * numRepeats, (i+1) / (GLfloat) nmax * numRepeats);
                glVertex3f(y, z1, x);
            }
            radius += nmax / 100000.;
            glTranslatef(0, -0.5, 0); // Narrow the branch
            glEnd();

            branchLeafIDX++;
        }

        //  South pole cap
        if (capON)
        {
            glPushMatrix();
                glTranslatef(0, height*0.48, 0);
                glScalef(radius*0.95, 0.15, radius*0.95);
                glPushMatrix();
                    glBegin(GL_TRIANGLE_FAN);
                    glRotatef(180, 0, 1, 0);
                    int d = 10;
                    Vertex(0, -90);
                    for (int th=0; th<=360; th+=d)
                    {
                        glTexCoord2f(th / (GLfloat) 360 * numRepeats, 0);
                        Vertex(th, d-90);
                    }
                    glEnd();
                glPopMatrix();

                //  Latitude bands
                glPushMatrix();
                    glRotatef(90, 0, 0, 1);
                    for (int ph=d-90; ph<=90-2*d; ph+=d)
                    {
                        glBegin(GL_QUAD_STRIP);
                        for (int th=0; th<=180; th+=d)
                        {
                            glTexCoord2f(th / (GLfloat) 360 * numRepeats, (ph) / (GLfloat) 360 * numRepeats);
                            Vertex(th, ph);
                            glTexCoord2f(th / (GLfloat) 360 * numRepeats, (ph+1) / (GLfloat) 360 * numRepeats);
                            Vertex(th, ph+d);
                        }
                        glEnd();
                    }
                glPopMatrix();
            glPopMatrix();
        }

        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0, 0, 1);
        z0 = 0.5 * height - 0 * MAX;
        for (int k=0;k<=360;k+=10)
        {
            glTexCoord2f(0.5*Sin(k)+0.5,0.5*Cos(k)+0.5);
            glVertex3f(originalRadius*Sin(k), z0, originalRadius*Cos(k));
        }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0, 0, 1);
        for (int k=0;k<=360;k+=10)
        {
            glTexCoord2f(0.5*Sin(k)+0.5,0.5*Cos(k)+0.5);
            glVertex3f(originalRadius*Sin(k), z1, originalRadius*Cos(k));
        }
        glEnd();
    glPopMatrix();
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
}

void CactiClass::setupTextures()
{
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    GLuint shaderID = shaderProgramIDs[0];
    glUseProgram(shaderID);

    GLuint cacti_ID = glGetUniformLocation(shaderID, "texture");
    float time = 0.001*glutGet(GLUT_ELAPSED_TIME);
    GLuint timeID = glGetUniformLocation(shaderID, "time");
    glUniform1f(timeID, time);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
    glUniform1i(cacti_ID, 0);
}


void CactiClass::render(int i, int j, glm::vec3 v1, float updatedWindRotation)
{
    windRotation = updatedWindRotation;
    setupTextures();
    float length, newLength, radius;
    float heightDivisions, radialDivisions;
    float rotDeg, sizeScaleFactor;
    bool leftArmON, rightArmON;

    leftArmON = true;
    rightArmON = true;

    length = 2;
    radius = 0.2;
    heightDivisions = 5;
    radialDivisions = 10;
    rotDeg = 0;
    sizeScaleFactor = 1;

    if (i % 3 == 0 && j % 2 == 0)
    {
        rotDeg = 37;
        sizeScaleFactor = 1.5;
        rightArmON = false;
    }
    else if (i % 1 == 0 && j % 2 == 1)
    {
        rotDeg = 95;
        sizeScaleFactor = 2.0;
    }
    else if (i % 4 == 1 && j % 2 == 0)
    {
        rotDeg = 0;
        sizeScaleFactor = 2.2;
        leftArmON = false;
    }
    else if (i % 2 == 1 && j % 2 == 1)
    {
        rotDeg = 175;
        sizeScaleFactor = 1.8;
    }

    glPushMatrix();
        glTranslatef(v1.x, v1.y + length*0.48*sizeScaleFactor, v1.z);
        glRotatef(rotDeg, 0, 1, 0);
        glScalef(1, sizeScaleFactor, 1);
        glColor3f(1, 1, 1);

        createCylinder(heightDivisions, radialDivisions, length, radius, true);

        if (leftArmON)
        {
            glPushMatrix();
                glTranslatef(-length/4.0*0.75, -length*1/6, 0);
                glRotatef(90, 0, 0, 1);
                glScalef(1.0, 0.5, 1.0);
                createCylinder(heightDivisions, radialDivisions, length/2.0, radius*0.65, false);
            glPopMatrix();

            glPushMatrix();
                newLength = length * 0.6;
                glTranslatef(-length/4.0, 0.0, 0);
                glScalef(1.0, 0.5, 1.0);
                createCylinder(heightDivisions, radialDivisions, newLength, radius*0.65, true);
            glPopMatrix();
        }

        if (rightArmON)
        {
            glPushMatrix();
                glTranslatef(length/4.0*0.75, -length*1/6*0.3, 0);
                glRotatef(-90, 0, 0, 1);
                glScalef(1.0, 0.5, 1.0);
                createCylinder(heightDivisions, radialDivisions, length/2.0, radius*0.65, false);
            glPopMatrix();

            glPushMatrix();
                glTranslatef(length/4.0, +0.2, 0);
                glScalef(1.0, 0.5, 1.0);
                createCylinder(heightDivisions, radialDivisions, newLength, radius*0.65, true);
            glPopMatrix();
        }

    glPopMatrix();
}
