#include "projectManager.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "TerrainClass.h"
#include "ShaderHandlerClass.h"
#include "TextureHandlerClass.h"
#include "SkyboxClass.h"
#include "WaterClass.h"
#include <string>

#include "stb_image_write.h"

using namespace std;

// GLOBALS
// =============================================================
int SCREEN_HEIGHT = 1000; //  Height
int SCREEN_WIDTH = 1200; //  Width

double dim = 10.0; //  Size of world

int rotateWorld = 1;
vector<float> rotateWorldRates = {0.01, 0.1, 0.5};
int rotateWorldRate_i = 0;

bool colorOnly = true;

int viewingMode = 0;

LightClass Light;
CameraClass Camera(dim);
TerrainClass Terrain;
ShaderHandlerClass ShaderHandler;
TextureHandlerClass TextureHandler;
SkyboxClass Skybox(dim*50);
WaterClass Water;
// =============================================================

// Function Prototypes
// --------------------------------------
void drawText();
void display();
void reshape(int, int);
void idle();
void captureScreen();
void checkGLError();
void specialKeyPressed(int, int, int);
void keyPressed(unsigned char, int, int);
// --------------------------------------


#define MAXLEN 8192  //  Maximum length of text string
void PrintTextToWindow(const char* format , ...)
{
    char    buf[MAXLEN];
    char*   ch=buf;
    va_list args;
    //  Turn the parameters into a character string
    va_start(args, format);
    vsnprintf(buf, MAXLEN, format, args);
    va_end(args);
    //  Display the characters one at a time at the current raster position
    while (*ch)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch++);
    }
}

void drawCameraLabel(int cameraNumber)
{
    glDisable(GL_TEXTURE_2D); // Ensure textures are turned off
    glColor3f(0,0,0);
    glWindowPos2i(5 + glutGet(GLUT_WINDOW_WIDTH)/3*cameraNumber, glutGet(GLUT_WINDOW_HEIGHT)/3-20);
    string cameraMode = (cameraNumber == 0) ? "Left" : (cameraNumber == 1) ? "Center" : "Right";
    PrintTextToWindow("Camera: %s", cameraMode.c_str());

}

void drawText()
{
    float yOffset = (viewingMode) ? SCREEN_HEIGHT/3 : 0;

    glDisable(GL_TEXTURE_2D); // Ensure textures are turned off

    glColor3f(0,0,0);
    glWindowPos2i(5, 5+yOffset);
    PrintTextToWindow("Terrain Spatial Scale: %f, Height Scale: %f", Terrain.spatialScale, Terrain.heightScale);
    glWindowPos2i(5, 25+yOffset);
    PrintTextToWindow("Height Bound=%s", Camera.bindCameraToTerrain ? "True" : "False");

    glWindowPos2i(5, 45+yOffset);
    PrintTextToWindow("Light: (%.2f, %.2f, %.2f)", Light.position[0], Light.position[1], Light.position[2]);

    glWindowPos2i(5, 85+yOffset);
    PrintTextToWindow("Terrain Shader; %s", Terrain.getShaderText().c_str());

    glWindowPos2i(5, glutGet(GLUT_WINDOW_HEIGHT)-20);
    string cameraMode = (Camera.cameraViewMode == 0) ? "Left" : (Camera.cameraViewMode == 1) ? "Center" : "Right";
    PrintTextToWindow("Camera: %s", cameraMode.c_str());
}

void checkGLError()
{
    GLenum err;
    while( !(err = glGetError()) ){
        cout << err << endl;
    }
}

void renderScene(int cameraNumber)
{
    glLoadIdentity(); //  Undo previous transformations

    Camera.render(cameraNumber); // Render camera with mode: first person, perspective, orthographic
    glm::vec3 camPosition = Camera.getPosition();
    Terrain.setCameraPosition(camPosition);
    if (Camera.bindCameraToTerrain)
    {
        float FPS_height = Terrain.getHeight();
        Camera.setFirstPersonHeight(FPS_height);
    }

    Skybox.render();
    Light.update();
    Terrain.render();
    Water.render();
    Light.disable();
}

void renderMainView()
{
    if (viewingMode)
    {
        Camera.resetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT*2/3);
        glViewport(0, SCREEN_HEIGHT/3, SCREEN_WIDTH, SCREEN_HEIGHT*2/3);
    }
    else
    {
        Camera.resetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    Camera.updateProjection(); //  Set projection
    renderScene(-1);
}

void renderCameraViews(int cameraNumber)
{
    int xPos = SCREEN_WIDTH/3*cameraNumber;
    Camera.resetAspectRatio(SCREEN_WIDTH/3, SCREEN_HEIGHT/3);
    Camera.updateProjection(); //  Set projection
    glViewport(xPos, 0, SCREEN_WIDTH/3, SCREEN_HEIGHT/3); // Set the viewport to the entire window
    renderScene(cameraNumber);
    drawCameraLabel(cameraNumber);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //  Erase the window and the depth buffer
    glEnable(GL_DEPTH_TEST); //  Enable Z-buffering in OpenGL
    renderMainView();

    if (viewingMode)
    {
        renderCameraViews(0);
        renderCameraViews(1);
        renderCameraViews(2);
    }

    drawText(); // Display debug info

    glFlush();
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    Camera.resetAspectRatio(width, height);
    SCREEN_WIDTH = width; // Set new screen width
    SCREEN_HEIGHT = height; // Set new screen height
    Camera.updateProjection(); //  Set projection
}

void idle()
{
    double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    float worldAzimuth = fmod(90 * t * rotateWorldRates[rotateWorldRate_i], 360.0);

    if (rotateWorld)
    {
        Light.updateAzimuthRotation(worldAzimuth);
        Skybox.updateAzimuthRotation(worldAzimuth);
    }
    Terrain.updateWind(t);
    glutPostRedisplay();
}

void specialKeyPressed(int key, int x, int y)
{
    Camera.specialKeyPressUpdate(key);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
    Camera.updateProjection();
}

void keyPressed(unsigned char key, int x, int y)
{
    Camera.keyPressUpdate(key);
    Light.keyPressUpdate(key, Camera.lookX, Camera.lookY, Camera.lookZ);

    switch (key)
    {
        case 27:
            exit(0);
            break;
        case 'g':
            Camera.toggleMode();
            break;
        case 'L':
            Light.toggleOn();
            break;
        case 'K':
            Light.toggleControl();
            break;
        case 't':
            Terrain.toggleSolidTerrain();
            break;
        case 'T':
            Terrain.toggleMeshWire();
            break;
        case 'S':
            Terrain.toggleShader();
            break;
        case 'W':
            Water.toggleShader();
            break;
        case 'r':
            rotateWorld = 1-rotateWorld;
            break;
        case 'R':
            rotateWorldRate_i++;
            rotateWorldRate_i %= (int)rotateWorldRates.size();
            break;
        case 'n':
            Terrain.toggleShowNormals();
            Water.toggleShowNormals();
            break;
        case '1':
            captureScreen();
            break;
        case '0':
            viewingMode = 1-viewingMode;
            break;
        case 32:
            Camera.startJump();
            break;
        case 'B':
            Terrain.toggleTextureColoring();
            break;
        case 'b':
            Terrain.clearCurrentBiome();
            Terrain.toggleBiome();
            TextureHandler.setTextures(Terrain.selectedBiomeTextureFiles, Terrain.selectedBiomeTextureIDs);
            break;
        case 'v':
            Terrain.toggleVegetation();
            break;
    }

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
    {
        Camera.toggleMouseLook();
    }
}

void captureScreen()
{
    int components = 3;
    string outpath = "test.png";
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    // https://stackoverflow.com/questions/7564186/what-is-the-easiest-way-to-turn-an-array-of-rgb-values-in-c-into-an-image-file
    vector<unsigned char> pixels(width * height * components);
    glReadPixels(width/2, height/2, width, height, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);
    // checkGLError();
    cout << "Reading image pixels..." << endl;
    // stbi_write_png(outpath.c_str(), width, height, components, &pixels, width*4);
    stbi_write_bmp(outpath.c_str(), width, height, components, &pixels);
}

void initializeShaderPrograms()
{
    // Initialize Terrain Shaders
    ShaderHandler.setShaders(Terrain.alpineShaders, Terrain.alpineShaderIDs);
    ShaderHandler.setShaders(Terrain.desertShaders, Terrain.desertShaderIDs);
    ShaderHandler.setShaders(Terrain.grassShaderFiles, Terrain.grassShaderProgramIDs);

    // Initialize Water Shaders
    ShaderHandler.setShaders(Water.shaderFiles, Water.shaderProgramIDs);

    // Initialize Skybox Shaders
    ShaderHandler.setShaders(Skybox.shaderFiles, Skybox.shaderProgramIDs);

    // Print All shaders
    ShaderHandler.printLookup();
}

void initializeTextures()
{
    // Initialize Terrain Textures
    // ===========================
    TextureHandler.setTextures(Terrain.selectedBiomeTextureFiles, Terrain.selectedBiomeTextureIDs);
    TextureHandler.setTextures(Terrain.grassBillboardFiles, Terrain.grassBillboardIDs);
    TextureHandler.setTextures(Terrain.trailMapFiles, Terrain.trailTextureMapIDs, false);

    // Initialize Water Textures
    TextureHandler.setTextures(Water.textureFiles, Water.textureIDs);

    // Initialize Skybox Textures
    GLuint skyboxID = TextureHandler.loadCubeMapTexture(Skybox.textureFiles);
    Skybox.setTextureID(skyboxID);
    TextureHandler.setTextures(Skybox.textureFiles, Skybox.textureIDs);

    // Print all textures
    TextureHandler.printLookup();
}

int main(int argc, char* argv[])
{
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
    Camera.initialize();
    Light.initialize(rotateWorld);
    Terrain.initialize();
    Water.initialize();
    Terrain.setBiome();

    initializeShaderPrograms();
    initializeTextures();
    Terrain.setBiome();
    // ---------------------------------

    // readHeightmap();
    glutMainLoop(); //  Pass control to GLUT so it can interact with the user
    return 0;
}
