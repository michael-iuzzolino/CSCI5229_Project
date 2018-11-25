// CameraClass.cpp
#include "projectManager.h"
#include "CameraClass.h"

CameraClass::CameraClass(double worldDim)
{
    dim = worldDim;
}

void CameraClass::initialize()
{
    translationChangeRate_GOD_MODE = 10;
    rotationChangeRate_GOD_MODE = 5;
    translationChangeRate_FPS_MODE = 0.5;
    rotationChangeRate_FPS_MODE = 5;

    translationChangeRate = translationChangeRate_GOD_MODE;
    rotationChangeRate = rotationChangeRate_GOD_MODE;

    cameraViewMode = 1; // default at center

    mouseLook = 0;
    mouseMovementTimer = 0;
    mouseMovementTimerMax = 1500.0;
    xLookSensitivity = 1/100.0;
    yLookSensitivity = 1/100.0;
    xLookValLimit = 200;
    yLookValLimit = 200;

    FPS_heightAdjustment = 1.1;

    azimuthSeparation = 45;
    int initAzimuth = 125;
    for (int i=0; i < 3; i++)
    {
        azimuths[i] = initAzimuth+azimuthSeparation*i;
    }
    azimuth = azimuths[cameraViewMode];

    elevation = -15.0;
    X = -58.0;
    Y = 10.0;
    Z = -68.0;
    lookX = 0;
    lookY = 0;
    lookZ = 0;
    movementLookX = 0;
    movementLookY = 0;
    movementLookZ = 0;
    upX = 0;
    upY = Cos(elevation);
    upZ = 0;

    bindCameraToTerrain = 0;

    fov = 55;
    aspectRatio = 1;
}

void CameraClass::updateLook(float targetAzimuth)
{
    lookX = Sin(targetAzimuth) * Cos(elevation);
    lookY = Sin(elevation);
    lookZ = -Cos(targetAzimuth) * Cos(elevation);

    if (bindCameraToTerrain)
    {
        movementLookX = Sin(azimuths[1]) * Cos(elevation);
        movementLookY = Sin(azimuths[1]);
        movementLookZ = -Cos(azimuths[1]) * Cos(elevation);
    }
    else
    {
        movementLookX = lookX;
        movementLookY = lookY;
        movementLookZ = lookZ;
    }
}


void CameraClass::render(int cameraNumber)
{
    float renderAzimuth;

    renderAzimuth = (cameraNumber == -1) ? azimuths[cameraViewMode] : azimuths[cameraNumber];

    updateLook(renderAzimuth);

    if (bindCameraToTerrain)
    {
        Y = terrainHeight + FPS_heightAdjustment;
        Y += getJumpHeight();
    }
    gluLookAt(X, Y, Z,
        X + lookX, Y + lookY, Z + lookZ,
        0.0, Cos(elevation), 0.0);

    updateMouseLook(lastMouseX, lastMouseY, mouseMovementTimer);
}

void CameraClass::toggleMode()
{
    bindCameraToTerrain = 1 - bindCameraToTerrain;

    if (bindCameraToTerrain)
    {
        translationChangeRate = translationChangeRate_FPS_MODE;
        rotationChangeRate = rotationChangeRate_FPS_MODE;
    }
    else
    {
        translationChangeRate = translationChangeRate_GOD_MODE;
        rotationChangeRate = rotationChangeRate_GOD_MODE;
    }
}

void CameraClass::updateProjection()
{
    double nearPlane, farPlane;
    glMatrixMode(GL_PROJECTION); //  Tell OpenGL we want to manipulate the projection matrix
    glLoadIdentity(); //  Undo previous transformations
    nearPlane = dim / 500.0;
    farPlane = 100 * dim;
    gluPerspective(fov, aspectRatio, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW); //  Switch to manipulating the model matrix
    glLoadIdentity(); //  Undo previous transformations
}

void CameraClass::resetAspectRatio(int width, int height)
{
    aspectRatio = (height > 0) ? (double) width / height : 1; //  Ratio of the width to the height of the window
}

void CameraClass::updateMouseLookAzimuths(float sensitivityDecay)
{
    for (int i=0; i < 3; i++)
    {
        azimuths[i] += lastMouseX*xLookSensitivity * sensitivityDecay;
    }
    azimuth = azimuths[cameraViewMode];
}

void CameraClass::updateAzimuths(float azimuthDelta)
{
    for (int i=0; i < 3; i++)
    {
        azimuths[i] += azimuthDelta;
    }
    azimuth = azimuths[cameraViewMode];
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
    updateMouseLookAzimuths(sensitivityDecay);
    elevation += lastMouseY*yLookSensitivity * sensitivityDecay;
}

void CameraClass::keyPressUpdate(unsigned char key)
{
    switch (key)
    {
        case 'w':
            X += movementLookX * translationChangeRate;
            Y += movementLookY * translationChangeRate;
            Z += movementLookZ * translationChangeRate;
            break;
        case 's':
            X -= movementLookX * translationChangeRate;
            Y -= movementLookY * translationChangeRate;
            Z -= movementLookZ * translationChangeRate;
            break;
        case 'a':
            updateAzimuths(-rotationChangeRate);
            break;
        case 'd':
            updateAzimuths(+rotationChangeRate);
            break;
        case 'z':
            Y -= translationChangeRate;
            break;
        case 'c':
            Y += translationChangeRate;
            break;
        case 'q':
            X += movementLookZ * translationChangeRate;
            Z -= movementLookX * translationChangeRate;
            break;
        case 'e':
            X -= movementLookZ * translationChangeRate;
            Z += movementLookX * translationChangeRate;
            break;
        case 'm':
        case 'M':
            cameraViewMode++;
            cameraViewMode %= 3;
            break;
    }
}

glm::vec3 CameraClass::getPosition()
{
    return glm::vec3(X, Y, Z);
}

void CameraClass::specialKeyPressUpdate(int key)
{
    if (key == GLUT_KEY_LEFT)
    {
        updateAzimuths(-rotationChangeRate);
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        updateAzimuths(+rotationChangeRate);
    }
    else if (key == GLUT_KEY_UP)
    {
        elevation += rotationChangeRate;
    }
    else if (key == GLUT_KEY_DOWN)
    {
        elevation -= rotationChangeRate;
    }
}

void CameraClass::toggleMouseLook()
{
    mouseLook = 1-mouseLook;
}

void CameraClass::setFirstPersonHeight(float newHeight)
{
    terrainHeight = newHeight;
}

void CameraClass::startJump()
{
    if (jumping) { return; }
    jumpStartTime = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    jumpHeight = Y;
    jumping = true;
}

float CameraClass::getJumpHeight()
{
    if (!jumping) { return 0; }
    float currentTime = glutGet(GLUT_ELAPSED_TIME)/1000.0 - jumpStartTime;
    float currentHeight = sin(currentTime) * 10;
    if (currentHeight < 0) {
        jumping = false;
        currentHeight = 0;
    }
    return currentHeight;
}
