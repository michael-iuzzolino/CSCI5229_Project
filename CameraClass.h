//CameraClass.h
#ifndef CameraClass_H
#define CameraClass_H

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

class CameraClass
{
public:
    int mouseLook;
    int lastMouseX, lastMouseY;
    float mouseMovementTimer, mouseMovementTimerMax;
    int xLookValLimit, yLookValLimit;
    float xLookSensitivity, yLookSensitivity;
    double FPS_heightAdjustment;
    double X, Y, Z; // Camera X, Y, Z positions
    glm::vec3 worldPosition;
    double lookX, lookY, lookZ; // Camera X, Y, Z look ats
    double upX, upY, upZ; // Camera X, Y, Z ups
    double azimuth, elevation, theta; // Camera angles
    int firstPersonMode; //  Toggle first person mode
    int projectionMode; //  Projection mode -- first person is always perspective
    int currentMode;
    int bindCameraToTerrain;
    int fov; //  Field of view (for perspective)
    double aspectRatio; //  Aspect ratio
    double dim;
    double translationChangeRate, rotationChangeRate;

    CameraClass(double, double, double);
    void init();
    void updateLook();
    void render(float);
    void toggleMode();
    void updateProjection();
    void resetAspectRatio(int, int);

    void updateMouseLook(int, int, float);
    void keyPressUpdate(unsigned char);
    void specialKeyPressUpdate(int);
    void toggleHeightBound();
    double queryTerrain(double, double);
    glm::vec3 getPosition();
    void setTerrainPosition(glm::vec3);
    glm::vec3 getTerrainPosition();
    void toggleMouseLook();
};
#endif
