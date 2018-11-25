//TerrainClass.h
#ifndef _TerrainClass_H
#define _TerrainClass_H
#include "projectManager.h"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "stb_image.h"

#include <string>
#include <fstream>
#include <istream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

class TerrainClass
{
public:
    vector<vector<float>> heightMapMesh;
    vector<vector<float>> trailsMapMesh;

    vector<vector<glm::vec3>> normalMap;
    int numRows, numCols, numVertices;
    float cx, cz;
    int currentShaderID;
    int numBiomeTextures;

    int heightmapWidth, heightmapHeight, heightmapSize;

    GLuint heightMapTextureID;

    // Flags
    // -------------------
    bool showFullTerrain;
    bool showMeshWire;
    bool showVegetation;
    bool showNormals;
    bool showTextureColorOnly;
    // -------------------

    float spatialScale;
    float heightScale;
    float heightAdjust;

    int grassRenderDistance;
    int maxGrassAltitude;
    int minGrassAltitude;
    float grassSeparation;

    float grassWindRotation;

    // HEIGHTMAP
    // ---------------------------------------------------
    string heightMapFile = "heightmap/heightmap_02.dat";
    // ---------------------------------------------------

    // BIOMES
    // ---------------------------------------------------
    string selectedBiome;
    int biomeIDX;
    vector<string> biomeNames = {"desert", "alpine"};

    vector<string> selectedBiomeTextureFiles;
    vector<GLuint> selectedBiomeTextureIDs_backup;
    vector<GLuint> selectedBiomeTextureIDs;
    vector<string> selectedBiomeShaderFiles;
    vector<GLuint> selectedBiomeShaderIDs;

    vector<string> alpineTextureFiles = {
        "terrain/alpine/sand.bmp",  "RGB",
        "terrain/alpine/grass.bmp", "RGB",
        "terrain/alpine/rock.bmp",  "RGB",
        "terrain/alpine/snow.bmp",  "RGB",
        "terrain/alpine/trail.bmp", "RGB"
    };

    vector<string> alpineShaders = {
        "terrain/alpine.vert",  "terrain/alpine.frag",
        "terrain/normals.vert", "terrain/normals.frag"
    };

    vector<GLuint> alpineShaderIDs;

    vector<string> desertTextureFiles = {
        "terrain/desert/beach_sand.bmp",     "RGB",
        "terrain/desert/sand.bmp",           "RGB",
        "terrain/desert/cracked_stone.bmp",  "RGB",
        "terrain/desert/mountain.bmp",       "RGB",
        "terrain/desert/white_sand.bmp",     "RGB"
    };

    vector<string> desertShaders = {
        "terrain/desert.vert",  "terrain/desert.frag",
        "terrain/normals.vert", "terrain/normals.frag"
    };

    vector<GLuint> desertShaderIDs;
    // ---------------------------------------------------

    // TRAIL
    // ---------------------------------------------------
    string trailsFile = "heightmap/heightmap_02_trails.dat";
    vector<string> trailMapFiles = {
        "terrain/trails2.bmp", "RGB"
    };
    vector<GLuint> trailTextureMapIDs;
    GLuint trailsTextureID;
    // ---------------------------------------------------

    // VEGETATION
    // ---------------------------------------------------
    vector<string> grassShaderFiles = {
        "terrain/grass.vert", "terrain/grass.frag"
    };
    vector<GLuint> grassShaderProgramIDs;

    vector<string> grassBillboardFiles = {
        "billboards/grass/grass_01.bmp", "RGB",
        "billboards/grass/grass_02.bmp", "RGB",
        "billboards/grass/grass_03.bmp", "RGB",
        "billboards/grass/grass_04.bmp", "RGB",
        "billboards/grass/grass_05.bmp", "RGBA"
    };
    vector<GLuint> grassBillboardIDs;
    GLuint grassTextureID;
    // ---------------------------------------------------



    glm::vec3 cameraPosition;

    TerrainClass();
    void initialize();
    void setBiome();
    void clearCurrentBiome();
    void heightMapBMPReader(string);
    void Reverse(void*, const int);
    void readHeightMapFile();
    void render();
    void calculateNormals();
    void renderGrass();
    void renderSolidTerrain();
    void updateWind(double);
    void toggleShader();

    void renderMeshWire();
    void toggleMeshWire();
    void toggleSolidTerrain();
    float getHeight();
    string getShaderText();
    void toggleShowNormals();
    void setCameraPosition(glm::vec3);
    void toggleBiome();
    void toggleVegetation();
    void toggleTextureColoring();
private:
    void cleanUpRender();
    void readTrailsFile();
    void setShaders();
    void setMaterials(string);
    void setTextures();
    void loadTrails();
    void renderGrassPatch(int, int);
    void renderGrassBlade(int, int, glm::vec3, float);
    void renderNormals();
    template <class T>
    void copyList(vector<T>, vector<T>&);
};
#endif
