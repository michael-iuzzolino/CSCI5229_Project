#include "projectManager.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "TerrainClass.h"
#include "ShaderHandlerClass.h"
#include "TextureHandlerClass.h"
#include "SkyboxClass.h"
#include "WaterClass.h"

// GLOBALS
// =============================================================
// Viewport params
int SCREEN_HEIGHT = 800; //  Height
int SCREEN_WIDTH = 1000; //  Width

// World dimension
double dim = 10.0; //  Size of world

// World Rotation Params
int rotateWorld = 1;
vector<float> rotateWorldRates = {0.01, 0.1, 0.25, 0.5};
int rotateWorldRate_i = 2;

// Single or multicam view
int viewingMode = 0;

// OPENGL runtime
float worldRuntime;

// Screen Recording Params
int imgCounter = 0;
bool recordModeON = 0;
float lastRecordTime = 0;
float recordModeFPS = 1;
float recordModeRate = 1/recordModeFPS;

// HUD params
vector<string> HUDshaderFiles = {
    "HUD/weather.vert", "HUD/weather.frag"
};
vector<GLuint> HUDshaderIDs;


// Instantiate Custom Classes
// --------------------------
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
void PrintTextToWindow(const char*, ...);
void drawCameraLabel(int);
void drawText();
void renderScene(int);
void renderMainView();
void renderCameraViews(int);
void renderHUD();
void display();
void reshape(int, int);
void idle();
void specialKeyPressed(int, int, int);
void keyPressed(unsigned char, int, int);
void motion(int, int);
void mouse(int, int, int, int);
void initializeShaderPrograms();
void initializeTextures();
void captureScreen();
int main(int, char*[]);
// --------------------------------------

void PrintTextToWindow(const char* format , ...)
{
    // Function written by Willem A. (Vlakkies) Schreüder
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
    PrintTextToWindow("Camera: %s, FOV: %0.1f", cameraMode.c_str(), Camera.getFOV());
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
    PrintTextToWindow("Light: (%.2f, %.2f, %.2f) -- Camera: (%.2f, %.2f, %.2f)", Light.position[0], Light.position[1], Light.position[2], Camera.X, Camera.Y, Camera.Z);

    glWindowPos2i(5, 85+yOffset);
    PrintTextToWindow("Terrain Shader; %s", Terrain.getShaderText().c_str());

    glWindowPos2i(5, glutGet(GLUT_WINDOW_HEIGHT)-20);
    string cameraMode = (Camera.cameraViewMode == 0) ? "Left" : (Camera.cameraViewMode == 1) ? "Center" : "Right";
    PrintTextToWindow("Camera: %s -- FOV: %0.1f", cameraMode.c_str(), Camera.getFOV());
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

void renderHUD()
{
    glUseProgram(HUDshaderIDs[0]);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint timeUniformID = glGetUniformLocation(HUDshaderIDs[0], "time");
    glUniform1f(timeUniformID, worldRuntime);


    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);

    glClear(GL_DEPTH_BUFFER_BIT);

    if (recordModeON)
    {
        glPushMatrix();
        float recRadius = 30;
        glColor3f(1, 0, 0);
        glTranslatef(SCREEN_WIDTH - recRadius*2, recRadius*2, 0);
        glScalef(recRadius, recRadius, 0);
        glBegin(GL_TRIANGLE_FAN);
        for (int k=0;k<=360;k+=10)
        {
            glVertex2f(Sin(k), Cos(k));
        }
        glEnd();
        glPopMatrix();
    }


    // Making sure we can render 3d again
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glUseProgram(0);
    glDisable(GL_BLEND);
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

    if (recordModeON)
    {
        captureScreen();
    }

    renderHUD();
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
    worldRuntime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    float worldAzimuth = fmod(90 * worldRuntime * rotateWorldRates[rotateWorldRate_i], 360.0);

    if (rotateWorld)
    {
        Light.updateAzimuthRotation(worldAzimuth);
        Skybox.updateAzimuthRotation(worldAzimuth);
        Water.setSkyboxRotationRate(rotateWorldRates[rotateWorldRate_i]);
    }
    else
    {
        Water.setSkyboxRotationRate(0);
    }
    Terrain.updateWind(worldRuntime);
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
            TextureHandler.setTextures(Terrain.selectedBiomeVegetationTextureFiles, Terrain.selectedBiomeVegetationTextureIDs);
            break;
        case 'v':
            Terrain.toggleVegetation();
            break;
        case 'p':
            Water.toggleShow();
            break;
        case 'N':
            Skybox.toggleShow();
            break;
        case 'H':
            Terrain.toggleHeightMap();
            break;
        case '_':
            Terrain.lowerHeight();
            break;
        case '+':
            Terrain.raiseHeight();
            break;
        case 'P':
            Terrain.generateNewHeightMap();
            break;
        case '[':
            Terrain.decreaseRenderDistance();
            break;
        case ']':
            Terrain.increaseRenderDistance();
            break;
        case 'm':
            Water.toggleWaterReflection();
            break;
        case 'y':
            recordModeON = !recordModeON;
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
    if (worldRuntime - lastRecordTime <= recordModeRate)
    {
        return;
    }
    lastRecordTime = worldRuntime;


    int components = 3;
    string outpath;
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    stbi_flip_vertically_on_write(true);

    if (viewingMode)
    {
        // int adjustedWidth = width / 3;
        // int adjustedHeight = height / 3;
        // GLubyte* pixels = (GLubyte*)malloc(components * adjustedWidth * adjustedHeight);
        //
        // // Left
        // outpath = "data_collection/left_camera__";
        // outpath += to_string(imgCounter);
        // outpath += ".bmp";
        // glReadPixels(0, SCREEN_HEIGHT/3, adjustedWidth, adjustedHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        // stbi_write_bmp(outpath.c_str(), adjustedWidth, adjustedHeight, components, pixels);
        //
        // // Center
        // outpath = "data_collection/center_camera__";
        // outpath += to_string(imgCounter);
        // outpath += ".bmp";
        // glReadPixels(SCREEN_WIDTH/3, SCREEN_HEIGHT/3, adjustedWidth, adjustedHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        // stbi_write_bmp(outpath.c_str(), adjustedWidth, adjustedHeight, components, pixels);
        //
        // // Right
        // outpath = "data_collection/right_camera__";
        // outpath += to_string(imgCounter);
        // outpath += ".bmp";
        // glReadPixels(SCREEN_WIDTH*2/3, SCREEN_HEIGHT/3, adjustedWidth, adjustedHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        // stbi_write_bmp(outpath.c_str(), adjustedWidth, adjustedHeight, components, pixels);

        GLubyte *pixels = (GLubyte *)malloc(components * width * height);
        outpath = "data_collection/single_camera__";
        outpath += to_string(imgCounter);
        outpath += ".bmp";
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        stbi_write_bmp(outpath.c_str(), width, height, components, pixels);

    }
    else
    {
        GLubyte *pixels = (GLubyte *)malloc(components * width * height);
        outpath = "data_collection/single_camera__";
        outpath += to_string(imgCounter);
        outpath += ".bmp";
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        stbi_write_bmp(outpath.c_str(), width, height, components, pixels);
    }

    imgCounter += 1;
}

void initializeShaderPrograms()
{
    // Initialize Terrain Shaders
    ShaderHandler.setShaders(Terrain.alpineShaders, Terrain.alpineShaderIDs);
    ShaderHandler.setShaders(Terrain.desertShaders, Terrain.desertShaderIDs);
    ShaderHandler.setShaders(Terrain.grassShaderFiles, Terrain.grassShaderProgramIDs);
    ShaderHandler.setShaders(Terrain.treeShaderFiles, Terrain.treeShaderProgramIDs);
    ShaderHandler.setShaders(Terrain.cactiShaderFiles, Terrain.cactiShaderProgramIDs);
    Terrain.trees.initializeShaderIDs(Terrain.treeShaderProgramIDs);
    Terrain.cacti.initializeShaderIDs(Terrain.cactiShaderProgramIDs);

    // Initialize Water Shaders
    ShaderHandler.setShaders(Water.shaderFiles, Water.shaderProgramIDs);

    // Initialize Skybox Shaders
    ShaderHandler.setShaders(Skybox.shaderFiles, Skybox.shaderProgramIDs);

    // HUD Shaders
    ShaderHandler.setShaders(HUDshaderFiles, HUDshaderIDs);

    // Print All shaders
    ShaderHandler.printLookup();
}

void initializeTextures()
{
    // Initialize Terrain Textures
    // ===========================
    TextureHandler.setTextures(Terrain.selectedBiomeTextureFiles, Terrain.selectedBiomeTextureIDs);
    TextureHandler.setTextures(Terrain.selectedBiomeVegetationTextureFiles, Terrain.selectedBiomeVegetationTextureIDs);
    TextureHandler.setTextures(Terrain.trailMapFiles, Terrain.trailTextureMapIDs, false);
    TextureHandler.setTextures(Terrain.treeTextureFiles, Terrain.treeTextureIDs);
    TextureHandler.setTextures(Terrain.cactiTextureFiles, Terrain.cactiTextureIDs);
    Terrain.trees.initializeTextureIDs(Terrain.treeTextureIDs);
    Terrain.cacti.initializeTextureIDs(Terrain.cactiTextureIDs);

    // Initialize Water Textures
    TextureHandler.setTextures(Water.textureFiles, Water.textureIDs);

    // Initialize Skybox Textures
    TextureHandler.loadCubeMapTexture(Skybox.textureFiles, Skybox.cubemapTextureID);

    // Print all textures
    TextureHandler.printLookup();
}

int main(int argc, char* argv[])
{
    // Core structure of function derived from Willem A. (Vlakkies) Schreüder's code for CSCI 5229 at CU Boulder

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

    Skybox.initCubeMap();
    Water.setSkyboxTextureID(Skybox.getCubemapTexture());
    // ---------------------------------

    // readHeightmap();
    glutMainLoop(); //  Pass control to GLUT so it can interact with the user

    return 0;
}
