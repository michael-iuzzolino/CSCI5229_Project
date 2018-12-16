//TerrainClass.h
#ifndef _TerrainClass_H
#define _TerrainClass_H
#include "projectManager.h"

#include "TreeClass.h"
#include "CactiClass.h"

class TerrainClass
{
public:
    vector<vector<float>> heightMapMesh;
    vector<vector<float>> trailsMapMesh;

    vector<vector<glm::vec3>> normalMap;
    int numVertices;
    float cx, cz;
    int currentShaderID;
    int numBiomeTextures;

    float camX, camZ;

    int heightmapWidth, heightmapHeight, heightmapSize;

    GLuint heightMapTextureID;

    mt19937 randomNumberGenerator;
    bool PerlinModeON;

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

    float windRotation;

    siv::PerlinNoise perlin;

    // HEIGHTMAP
    // ---------------------------------------------------
    int heightMapIDX;
    string currentHeightMapFile;
    vector<string> heightMaps = {"heightmap/heightmap_02.dat", "heightmap/heightmap_01.dat", "heightmap/heightmap_03.dat", "heightmap/heightmap_04.dat"};
    // ---------------------------------------------------

    // TREES
    // ---------------------------------------------------
    TreeClass trees;
    vector<GLuint> treeTextureIDs;
    vector<string> treeTextureFiles = {
        "trees/bark.bmp",   "RGB",
        "trees/leaf1.bmp",  "RGB",
        "trees/leaf2.bmp",  "RGB",
        "trees/leaf3.bmp",  "RGB"
    };

    vector<string> treeShaderFiles = {
        "trees/leaves.vert", "trees/leaves.frag"
    };
    vector<GLuint> treeShaderProgramIDs;
    // ---------------------------------------------------

    // CACTI
    // ---------------------------------------------------
    CactiClass cacti;
    vector<GLuint> cactiTextureIDs;
    vector<string> cactiTextureFiles = {
        "cacti/cacti.bmp",   "RGB"
    };

    vector<string> cactiShaderFiles = {
        "cacti/cacti.vert", "cacti/cacti.frag"
    };
    vector<GLuint> cactiShaderProgramIDs;
    // ---------------------------------------------------

    // BIOMES
    // ---------------------------------------------------
    string selectedBiome;
    int biomeIDX;
    vector<string> biomeNames = {"alpine", "desert"};

    vector<string> selectedBiomeTextureFiles;
    vector<GLuint> selectedBiomeTextureIDs_backup;
    vector<GLuint> selectedBiomeTextureIDs;
    vector<string> selectedBiomeVegetationTextureFiles;
    vector<GLuint> selectedBiomeVegetationTextureIDs;
    vector<string> selectedBiomeShaderFiles;
    vector<GLuint> selectedBiomeShaderIDs;

    vector<string> alpineTextureFiles = {
        "terrain/alpine/sand.bmp",  "RGB",
        "terrain/alpine/grass.bmp", "RGB",
        "terrain/alpine/rock.bmp",  "RGB",
        "terrain/alpine/snow.bmp",  "RGB",
        "terrain/alpine/trail.bmp", "RGB"
    };

    vector<string> alpineVegetationTextureFiles = {
        "billboards/grass/grass_01.bmp", "RGB",
        "billboards/grass/grass_02.bmp", "RGB",
        "billboards/grass/grass_03.bmp", "RGB",
        "billboards/grass/grass_04.bmp", "RGB",
        "billboards/grass/grass_05.bmp", "RGBA"
    };

    vector<string> alpineShaders = {
        "terrain/alpine.vert",  "terrain/alpine.frag",
        "terrain/normals.vert", "terrain/normals.frag",
        "terrain/normals_2.vert", "terrain/normals_2.frag"
    };

    vector<GLuint> alpineShaderIDs;

    vector<string> desertTextureFiles = {
        "terrain/desert/beach_sand.bmp",     "RGB",
        "terrain/desert/sand.bmp",           "RGB",
        "terrain/desert/cracked_stone.bmp",  "RGB",
        "terrain/desert/mountain.bmp",       "RGB",
        "terrain/desert/white_sand.bmp",     "RGB"
    };

    vector<string> desertVegetationTextureFiles = {
        "billboards/desert/plant_04.bmp", "RGB",
        "billboards/desert/plant_04.bmp", "RGB",
        "billboards/desert/plant_04.bmp", "RGB",
        "billboards/desert/plant_03.bmp", "RGB",
        "billboards/desert/plant_04.bmp", "RGB",
    };

    vector<string> desertShaders = {
        "terrain/desert.vert",  "terrain/desert.frag",
        "terrain/normals.vert", "terrain/normals.frag",
        "terrain/normals_2.vert", "terrain/normals_2.frag"
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
    void renderVegetation();
    void renderSolidTerrain();
    void renderTreesOrCacti(int, int);
    void updateWind(double);
    void toggleShader();
    void generateNewHeightMap();

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
    void setBiomeTextures();
    void setBiomeShaders();
    void toggleHeightMap();
    void lowerHeight();
    void raiseHeight();
    void increaseRenderDistance();
    void decreaseRenderDistance();
    void updatePerlinMap();
private:
    float maxPixelDistance;
    float minTreeAltitude, maxTreeAltitude;
    int PerlinRows, PerlinCols;
    float deltaHeight;
    void initPerlinMap();
    float readHeightMap(int, int);
    void cleanUpRender();
    void readTrailsFile();
    void setShaders();
    void setMaterials(string);
    void setTextures();
    void loadTrails();
    void renderGrass(int, int, float);
    void renderGrassBlade(int, int, glm::vec3, float);
    void renderNormals();
    template <class T>
    void copyList(vector<T>, vector<T>&);
};
#endif
