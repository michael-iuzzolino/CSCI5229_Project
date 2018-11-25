//LightClass.h
#ifndef LightClass_H
#define LightClass_H
class LightClass
{
public:
    int fixedToSkybox;
    double initAzimuth, azimuth;
    double sourceRadius;
    double smoothShading;
    float position[4];
    int showLight;

    double X, Y, Z;
    int lightingOn, controlMode;
    double userMoveRate, radius, height;
    double emission;        // Emission intensity (%)
    double ambient;         // Ambient intensity (%)
    double diffuse;         // Diffuse intensity (%)
    double specular;        // Specular intensity (%)
    double shininess;       // Shininess (power of two)
    double shiny;           // Shininess (value)

    double translationChangeRate, rotationChangeRate;

    LightClass();
    void initialize(int);
    void update();
    void updateAzimuthRotation(float);
    void render();
    void keyPressUpdate(unsigned char, double, double, double);
    void toggleOn();
    void toggleControl();
    void disable();
};
#endif
