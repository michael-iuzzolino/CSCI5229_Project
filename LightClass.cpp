// LightClass.cpp
#include "projectManager.h"
#include "LightClass.h"

LightClass::LightClass() {}

void LightClass::initialize(int fixed)
{
    translationChangeRate = 1;
    rotationChangeRate = 5;
    fixedToSkybox = fixed;
    if (fixedToSkybox)
    {
        sourceRadius = 5;
        radius = 150.5;
        height = 40;
        initAzimuth = 80;
    }
    else
    {
        sourceRadius = 8;
        radius = 300.5;
        height = 15;
    }

    smoothShading = true;
    lightingOn = true;
    userControlMode = false;
    userMoveRate = 5;

    emission = 0;
    ambient = 30;
    diffuse = 100;
    specular = 10;
    shininess = 0;
    shiny = 1;

    X = 0;
    Y = height;
    Z = 0;
}

void LightClass::updateAzimuthRotation(float newAzimuth)
{
    azimuth = newAzimuth + initAzimuth;
}

void LightClass::update()
{
    //  Flat or smooth shading
    glShadeModel(smoothShading ? GL_SMOOTH : GL_FLAT);

    if (userControlMode)
    {
        position[0] = X;
        position[1] = Y;
        position[2] = Z;
        position[3] = 1;
    }
    else
    {
        position[0] = radius * Cos(azimuth);
        position[1] = 2 * Cos(azimuth/2.0) + height;
        position[2] = radius * Sin(azimuth);
        position[3] = 1;
    }

    render();

    if (lightingOn)
    {
        float Ambient[]   = {0.01*ambient, 0.01*ambient, 0.01*ambient,1.0};
        float Diffuse[]   = {0.01*diffuse, 0.01*diffuse, 0.01*diffuse,1.0};
        float Specular[]  = {specular,specular,specular,1.0};

        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Enable light 0
        glEnable(GL_LIGHT0);

        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT , Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE , Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR, Specular);
        glLightfv(GL_LIGHT0,GL_POSITION, position);
    }
    else
    {
        glDisable(GL_LIGHTING);
    }
}

void LightClass::render()
{
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(position[0], position[1], position[2]);
    glScaled(sourceRadius, sourceRadius, sourceRadius);
    //  White ball
    glColor3f(1, 1, 0);
    if (showLight)
    {
        glutSolidSphere(1.0, 16, 16);
    }
    //  Undo transformations
    glPopMatrix();
}

void LightClass::keyPressUpdate(unsigned char key, double lookX, double lookY, double lookZ)
{
    if (!userControlMode)
    {
        return;
    }

    switch (key)
    {
        case 'i':
            X += lookX * userMoveRate;
            Z += lookZ * userMoveRate;
            break;
        case 'k':
            X -= lookX * userMoveRate;
            Z -= lookZ * userMoveRate;
            break;
        case 'j':
            X += lookZ * userMoveRate;
            Z -= lookX * userMoveRate;
            break;
        case 'l':
            X -= lookZ * userMoveRate;
            Z += lookX * userMoveRate;
            break;
        case 'u':
            Y -= userMoveRate;
            break;
        case 'o':
            Y += userMoveRate;
            break;
    }
}

void LightClass::toggleOn()
{
    lightingOn = !lightingOn;
}

void LightClass::toggleControl()
{
    userControlMode = !userControlMode;
    showLight = userControlMode;
}

void LightClass::disable()
{
    glDisable(GL_LIGHTING);
}
