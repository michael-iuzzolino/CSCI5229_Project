// CameraClass.cpp
#include "projectManager.h"
#include "CameraClass.h"

CameraClass::CameraClass(double worldDim, double worldTranslationChangeRate, double worldRotationChangeRate)
{
    dim = worldDim;
    translationChangeRate = worldTranslationChangeRate;
    rotationChangeRate = worldRotationChangeRate;
}

void CameraClass::init()
{
    mouseLook = 0;
    mouseMovementTimer = 0;
    mouseMovementTimerMax = 1500.0;
    xLookSensitivity = 1/100.0;
    yLookSensitivity = 1/100.0;
    xLookValLimit = 200;
    yLookValLimit = 200;

    FPS_heightAdjustment = 0.005;
    azimuth = 0.0;
    elevation = 2.0;
    X = 0.0;
    Y = 2.0;
    Z = 7.0;
    lookX = 0;
    lookY = 0;
    lookZ = -1.0;
    upX = 0;
    upY = Cos(elevation);
    upZ = 0;

    bindCameraToTerrain = 0;

    fov = 55;
    aspectRatio = 1;
    currentMode = 0;
    projectionMode = 1;
    firstPersonMode = 1;
}

void CameraClass::updateLook()
{
    lookX = Sin(azimuth) * Cos(elevation);
    lookY = Sin(elevation);
    lookZ = -Cos(azimuth) * Cos(elevation);
}


void CameraClass::render(float terrainHeight)
{
    if (firstPersonMode)
    {
        updateLook();

        if (bindCameraToTerrain)
        {
            Y = terrainHeight + FPS_heightAdjustment;
        }
        gluLookAt(X, Y, Z,
            X + lookX, Y + lookY, Z + lookZ,
            0.0, Cos(elevation), 0.0);
    }
    else if (projectionMode)
    {
        double Ex = -2 * dim * Sin(azimuth) * Cos(elevation);
        double Ey = +2 * dim * Sin(elevation);
        double Ez = +2 * dim * Cos(azimuth) * Cos(elevation);
        gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, Cos(elevation), 0);
    }
    else
    {
        glRotatef(elevation, 1, 0, 0);
        glRotatef(azimuth, 0, 1, 0);
    }


    updateMouseLook(lastMouseX, lastMouseY, mouseMovementTimer);

}

void CameraClass::toggleMode()
{
    currentMode++;
    currentMode %= 3;

    if (currentMode == 0)
    {
        firstPersonMode = 1;
        projectionMode = 1;
        init();
    }
    else
    {
        firstPersonMode = 0;
        projectionMode = (currentMode == 1) ? 1 : 0;
    }
}

void CameraClass::updateProjection()
{
    double nearPlane, farPlane;
    glMatrixMode(GL_PROJECTION); //  Tell OpenGL we want to manipulate the projection matrix
    glLoadIdentity(); //  Undo previous transformations
    if (firstPersonMode) {
        nearPlane = dim / 100.0;
        farPlane = 100.5 * dim;
        gluPerspective(fov, aspectRatio, nearPlane, farPlane);
    } else {
        if (projectionMode) //  Perspective transformation
        {
            nearPlane = dim / 4;
            farPlane = 4 * dim;
            gluPerspective(fov, aspectRatio, nearPlane, farPlane);
        } else //  Orthogonal projection
            glOrtho(-aspectRatio * dim, +aspectRatio * dim, -dim, +dim, -dim, +dim);
    }

    glMatrixMode(GL_MODELVIEW); //  Switch to manipulating the model matrix
    glLoadIdentity(); //  Undo previous transformations
}

void CameraClass::resetAspectRatio(int width, int height)
{
    aspectRatio = (height > 0) ? (double) width / height : 1; //  Ratio of the width to the height of the window
}

void CameraClass::updateMouseLook(int x, int y, float movementTimer)
{
    if (!mouseLook)
        return;

    mouseMovementTimer += 1;

    movementTimer = (movementTimer > mouseMovementTimerMax) ? mouseMovementTimerMax : movementTimer;

    x = glm::max(-xLookValLimit, glm::min(x, xLookValLimit));
    y = glm::max(-yLookValLimit, glm::min(y, yLookValLimit));

    lastMouseX = x;
    lastMouseY = y;
    float sensitivityDecay = (mouseMovementTimerMax - movementTimer)/mouseMovementTimerMax;
    azimuth += lastMouseX*xLookSensitivity * sensitivityDecay;
    elevation += lastMouseY*yLookSensitivity * sensitivityDecay;
}

void CameraClass::keyPressUpdate(unsigned char key)
{
    if (firstPersonMode)
    {
        if (key == 'w') // Move Forward
        {
            X += lookX * translationChangeRate;
            Y += lookY * translationChangeRate;
            Z += lookZ * translationChangeRate;
        }
        else if (key == 's') // Move Backwards
        {
            X -= lookX * translationChangeRate;
            Y -= lookY * translationChangeRate;
            Z -= lookZ * translationChangeRate;
        }
        else if (key == 'a') // Look left
            azimuth -= rotationChangeRate;

        else if (key == 'd') // Look Right
            azimuth += rotationChangeRate;

        else if (key == 'z') // Move down in Y
            Y -= translationChangeRate;
        else if (key == 'c') // Move up in Y
            Y += translationChangeRate;
        else if (key == 'q') // Strafe Left
        {
            X += lookZ * translationChangeRate;
            Z -= lookX * translationChangeRate;
        }
        else if (key == 'e') // Strafe Right
        {
            X -= lookZ * translationChangeRate;
            Z += lookX * translationChangeRate;
        }
    }
    else
    {
        if (key == 'a')
            azimuth += rotationChangeRate;
        else if (key == 'd')
            azimuth -= rotationChangeRate;
        else if (key == 'w')
            elevation += rotationChangeRate;
        else if (key == 's')
            elevation -= rotationChangeRate;

        elevation = elevation < 1 ? 1 : elevation; // Elevation: Restrict from going under ground
        elevation = elevation > 90 ? 90 : elevation; // Elevation: Restrict camera from flipping upside down
        azimuth = fmod(azimuth, 360); // Azimuth: mod to 360
    }
}

glm::vec3 CameraClass::getPosition()
{
    return glm::vec3(X, Y, Z);
}

void CameraClass::specialKeyPressUpdate(int key)
{
    if (firstPersonMode)
    {
        if (key == GLUT_KEY_LEFT)
            azimuth -= rotationChangeRate;
        else if (key == GLUT_KEY_RIGHT)
            azimuth += rotationChangeRate;
        else if (key == GLUT_KEY_UP)
            elevation += rotationChangeRate;
        else if (key == GLUT_KEY_DOWN)
            elevation -= rotationChangeRate;
    }
    else // Perspective or Ortho
    {
        if (key == GLUT_KEY_LEFT)
            azimuth += rotationChangeRate;
        else if (key == GLUT_KEY_RIGHT)
            azimuth -= rotationChangeRate;
        else if (key == GLUT_KEY_UP)
            elevation += rotationChangeRate;
        else if (key == GLUT_KEY_DOWN)
            elevation -= rotationChangeRate;

        elevation = elevation < 1 ? 1 : elevation; // Elevation: Restrict from going under ground
        elevation = elevation > 90 ? 90 : elevation; // Elevation: Restrict camera from flipping upside down
        azimuth = fmod(azimuth, 360); // Azimuth: mod to 360
    }
}

void CameraClass::toggleHeightBound()
{
    bindCameraToTerrain = 1 - bindCameraToTerrain;
}

void CameraClass::setTerrainPosition(glm::vec3 newWorldPosition)
{
    worldPosition = newWorldPosition;
}

glm::vec3 CameraClass::getTerrainPosition()
{
    return worldPosition;
}

void CameraClass::toggleMouseLook()
{
    mouseLook = 1-mouseLook;
}
