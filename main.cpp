#include "projectManager.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "TerrainClass.h"
#include "ShaderHandlerClass.h"
#include "SkyboxClass.h"
#include <string>

using namespace std;

// GLOBALS
// =============================================================
int SCREEN_HEIGHT = 800; //  Height
int SCREEN_WIDTH = 800; //  Width

double dim = 10.0; //  Size of world
double translationChangeRate = 1;
double rotationChangeRate = 5;

LightClass Light(translationChangeRate, rotationChangeRate);
CameraClass Camera(dim, translationChangeRate, rotationChangeRate);
TerrainClass Terrain;
ShaderHandlerClass ShaderHandler;
SkyboxClass Skybox(dim*50);
// =============================================================

// Function Prototypes
// --------------------------------------
void drawText();
void display();
void reshape(int, int);
void idle();
void specialKeyPressed(int, int, int);
void keyPressed(unsigned char, int, int);
// --------------------------------------

void drawText() {
    glColor3f(0,0,0);
    glWindowPos2i(5, 5);
    Print("View Mode=%s -- ShaderMode=%s -- Terrain Spatial Scale: %f, Height Scale: %f",
        Camera.firstPersonMode ? "First Person" : (Camera.projectionMode ? "Perpective" : "Orthogonal"),
        ShaderHandler.getText().c_str(),
        Terrain.scale, Terrain.heightScale
    );
    glWindowPos2i(5, 25);
    Print("Height Bound=%s", Camera.bindCameraToTerrain ? "True" : "False");

    glWindowPos2i(5, 45);
    Print("Light: (%.2f, %.2f, %.2f)", Light.position[0], Light.position[1], Light.position[2]);

    glWindowPos2i(5, 65);
    glm::vec3 camPos = Camera.getPosition();
    glm::vec3 camTerrainPosition = Camera.getTerrainPosition();
    Print("Camera: (%.2f, %.2f, %.2f) -- Camera Terrain Position: (%.2f, %.2f, %.2f)",
        camPos.x, camPos.y, camPos.z,
        camTerrainPosition.x, camTerrainPosition.y, camTerrainPosition.z);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //  Erase the window and the depth buffer
    glEnable(GL_DEPTH_TEST); //  Enable Z-buffering in OpenGL
    glLoadIdentity(); //  Undo previous transformations

    glm::vec3 camTerrainPosition = Terrain.getHeight(Camera.getPosition());
    Camera.setTerrainPosition(camTerrainPosition);
    Camera.render(camTerrainPosition.y); // Render camera with mode: first person, perspective, orthographic
    Skybox.render();
    Light.update();

    ShaderHandler.enable();
    GLuint terrainShaderID = ShaderHandler.getShaderID();
    Terrain.initializeShader(terrainShaderID);
    Terrain.render();
    ShaderHandler.disable();
    Light.disable();

    drawText(); // Display debug info

    glFlush();
    glutSwapBuffers();
}

void reshape(int width, int height) {
    Camera.resetAspectRatio(width, height);
    SCREEN_WIDTH = width; // Set new screen width
    SCREEN_HEIGHT = height; // Set new screen height
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Set the viewport to the entire window
    Camera.updateProjection(); //  Set projection
}

void idle() {
    double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    Light.azimuth = fmod(90 * t, 360.0);
    glutPostRedisplay();
}

void specialKeyPressed(int key, int x, int y) {
    Camera.specialKeyPressUpdate(key);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
    Camera.updateProjection();
}

void keyPressed(unsigned char key, int x, int y) {
    Camera.keyPressUpdate(key);
    Light.keyPressUpdate(key, Camera.lookX, Camera.lookY, Camera.lookZ);

    if (key == 27)
        exit(0);
    else if (key == 'm' || key == 'M')
        Camera.toggleMode();
    else if (key == 'L')
        Light.toggleOn();
    else if (key == 'K')
        Light.toggleControl();
    else if (key == 'B')
        Camera.toggleHeightBound();
    else if (key == 't')
        Terrain.toggleSolidTerrain();
    else if (key == 'T')
        Terrain.toggleMeshWire();
    else if (key == '-')
        ShaderHandler.decrementMode();
    else if (key == '=')
        ShaderHandler.incrementMode();
    else if (key == 'S')
        ShaderHandler.toggle();
    else if (key == '1')
        Terrain.decreaseHeightScale();
    else if (key == '2')
        Terrain.increaseHeightScale();
    else if (key == '3')
        Terrain.decreaseSpatialScale();
    else if (key == '4')
        Terrain.increaseSpatialScale();

    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
    Camera.updateProjection();
}

void motion(int x, int y)
{
    Camera.updateMouseLook(x - SCREEN_WIDTH/2, -(y - SCREEN_HEIGHT/2), 0);
}

void mouse(int key, int status, int x, int y)
{
    if (status==GLUT_DOWN) //  On button down, set 'move' and remember location
        Camera.toggleMouseLook();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv); //  Initialize GLUT
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE); //  Request float buffered, true color window with Z buffering at 600x600
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT); // Init window size
    glutCreateWindow("Project: Michael Iuzzolino"); // Init window

#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif

    //  Set callbacks
    // ---------------------------------
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeyPressed);
    glutKeyboardFunc(keyPressed);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    // ---------------------------------

    // Initialize objects
    // ---------------------------------
    Camera.init();
    Light.init();
    Skybox.initialize();
    Terrain.initialize();
    ShaderHandler.initialize();
    // ---------------------------------

    // readHeightmap();
    glutMainLoop(); //  Pass control to GLUT so it can interact with the user
    return 0;
}
