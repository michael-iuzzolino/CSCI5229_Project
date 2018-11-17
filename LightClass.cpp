// LightClass.cpp
#include "projectManager.h"
#include "LightClass.h"

LightClass::LightClass(double worldTranslationChangeRate, double worldRotationChangeRate)
{
    translationChangeRate = worldTranslationChangeRate;
    rotationChangeRate = worldRotationChangeRate;
}

void LightClass::init()
{
    sourceRadius = 1;
    smoothShading = 1;
    lightingOn = 1;
    controlMode = 0;
    moveRate = 0.1;
    radius = 3.5;
    height = 0.4;
    emission = 0;
    ambient = 30;
    diffuse = 100;
    specular = 0;
    shininess = 0;
    shiny = 1;

    X = 0;
    Y = height;
    Z = 0;
}

void LightClass::update()
{
    //  Flat or smooth shading
    glShadeModel(smoothShading ? GL_SMOOTH : GL_FLAT);

    if (controlMode)
    {
        position[0] = X;
        position[1] = Y;
        position[2] = Z;
        position[3] = 1;
    }
    else
    {
        position[0] = radius * Cos(azimuth);
        position[1] = height;
        position[2] = radius * Sin(azimuth);
        position[3] = 1;
    }

    render();

    if (lightingOn)
    {
        float Emission[]  = {0.0,0.0,0.0,1.0};
        float Ambient[]   = {0.01*ambient, 0.01*ambient, 0.01*ambient,1.0};
        float Diffuse[]   = {0.01*diffuse, 0.01*diffuse, 0.01*diffuse,1.0};
        float Specular[]  = {specular,specular,specular,1.0};
        float Shinyness[] = {16};
        float RGBA[] = {1,1,1,1};

        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,position);
        //  Set materials
        glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,RGBA);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,RGBA);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
        glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

        //
        // GLfloat Ambient[] = {
        //     0.01 * ambient,
        //     0.01 * ambient,
        //     0.01 * ambient,
        //     1.0
        // };
        // GLfloat Diffuse[] = {
        //     0.01 * diffuse,
        //     0.01 * diffuse,
        //     0.01 * diffuse,
        //     1.0
        // };
        // GLfloat Specular[] = {
        //     0.01 * specular,
        //     0.01 * specular,
        //     0.01 * specular,
        //     1.0
        // };
        //
        // glEnable(GL_NORMALIZE);
        // //  Enable lighting
        // glEnable(GL_LIGHTING);
        // //  glColor sets ambient and diffuse color materials
        // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        // glEnable(GL_COLOR_MATERIAL);
        // //  Enable light 0
        // glEnable(GL_LIGHT0);
        // //  Set ambient, diffuse, specular components and position of light 0
        // glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
        // glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
        // glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
        // glLightfv(GL_LIGHT0, GL_POSITION, position);
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
    glutSolidSphere(1.0, 16, 16);
    //  Undo transformations
    glPopMatrix();
}

void LightClass::keyPressUpdate(unsigned char key, double lookX, double lookY, double lookZ)
{
    if (!controlMode)
    {
        return;
    }

    if (key == 'i') {
        X += lookX * moveRate;
        Z += lookZ * moveRate;
    } else if (key == 'k') {
        X -= lookX * moveRate;
        Z -= lookZ * moveRate;
    } else if (key == 'j') {
        X += lookZ * moveRate;
        Z -= lookX * moveRate;
    } else if (key == 'l') {
        X -= lookZ * moveRate;
        Z += lookX * moveRate;
    } else if (key == 'u') {
        Y -= moveRate;
    } else if (key == 'o') {
        Y += moveRate;
    }
}

void LightClass::toggleOn()
{
    lightingOn = 1 - lightingOn;
}

void LightClass::toggleControl()
{
    controlMode = 1 - controlMode;
}

void LightClass::disable()
{
    glDisable(GL_LIGHTING);
}
