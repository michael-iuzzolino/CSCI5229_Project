//CameraClass.h
#ifndef CameraClass_H
#define CameraClass_H

class CameraClass
{
public:
    int mouseLook;
    int lastMouseX, lastMouseY;
    float mouseMovementTimer, mouseMovementTimerMax;
    float terrainHeight;
    int xLookValLimit, yLookValLimit;
    float xLookSensitivity, yLookSensitivity;
    double FPS_heightAdjustment;
    double X, Y, Z; // Camera X, Y, Z positions
    glm::vec3 worldPosition;
    double lookX, lookY, lookZ; // Camera X, Y, Z look ats
    double movementLookX, movementLookY, movementLookZ;
    double upX, upY, upZ; // Camera X, Y, Z ups
    double azimuth, elevation, theta; // Camera angles
    glm::vec3 azimuths;
    float azimuthSeparation;
    int currentMode;
    bool bindCameraToTerrain;
    int fov; //  Field of view (for perspective)
    double aspectRatio; //  Aspect ratio
    double dim;
    double translationChangeRate, rotationChangeRate;

    float translationChangeRate_GOD_MODE;
    float rotationChangeRate_GOD_MODE;
    float translationChangeRate_FPS_MODE;
    float rotationChangeRate_FPS_MODE;

    bool jumping;
    float jumpStartTime;
    float jumpHeight;

    int cameraViewMode; // 0: left, 1: center, 2: right

    CameraClass(double);
    void initialize();
    void updateLook(float);
    void render(int);
    void toggleMode();
    void updateProjection();
    void resetAspectRatio(int, int);
    void setFirstPersonHeight(float);

    void updateMouseLook(int, int, float);
    void keyPressUpdate(unsigned char);
    void specialKeyPressUpdate(int);
    double queryTerrain(double, double);
    glm::vec3 getPosition();
    void toggleMouseLook();
    float getFOV();
    void startJump();
private:
    void updateMouseLookAzimuths(float);
    void updateAzimuths(float);
    float getJumpHeight();
};
#endif
