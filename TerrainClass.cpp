// TerrainClass.cpp
#include "TerrainClass.h"

TerrainClass::TerrainClass()
{
    // INITIAL VALUES
    // ======================
    biomeIDX = 0;
    currentShaderID = 0;
    heightMapIDX = 0;

    heightScale = 0.165;

    PerlinRows = 512;
    PerlinCols = 512;

    maxPixelDistance = 256;

    mt19937 randomNumberGenerator{time(0)};

    heightAdjust = -10;
    deltaHeight = 1.0;

    grassRenderDistance = 15;
    maxGrassAltitude = 10;
    minGrassAltitude = 4.5;
    grassSeparation = 0.2;

    maxTreeAltitude = 15;
    minTreeAltitude = 5;
    // ======================

    // FLAGS
    // ======================
    PerlinModeON = false;
    showFullTerrain = true;
    showMeshWire = true;
    showVegetation = true;
    showNormals = false;
    showTextureColorOnly = false;
    // ======================
}

void TerrainClass::clearCurrentBiome()
{
    for (int i=0; i < (int) selectedBiomeTextureIDs.size(); i++)
    {
        glDeleteTextures(1, &selectedBiomeTextureIDs[i]);
    }

    for (int i=0; i < (int) selectedBiomeVegetationTextureIDs.size(); i++)
    {
        glDeleteTextures(1, &selectedBiomeVegetationTextureIDs[i]);
    }
}

template <class T>
void TerrainClass::copyList(vector<T> fromList, vector<T>& toList)
{
    toList.clear();
    for (int i=0; i < (int) fromList.size(); i++)
    {
        toList.push_back(fromList[i]);
    }
}

void TerrainClass::setBiomeTextures()
{
    if (selectedBiome == "alpine")
    {
        copyList<string>(alpineTextureFiles, selectedBiomeTextureFiles);
        copyList<string>(alpineVegetationTextureFiles, selectedBiomeVegetationTextureFiles);
    }
    else if (selectedBiome == "desert")
    {
        copyList<string>(desertTextureFiles, selectedBiomeTextureFiles);
        copyList<string>(desertVegetationTextureFiles, selectedBiomeVegetationTextureFiles);
    }
}

void TerrainClass::setBiomeShaders()
{
    if (selectedBiome == "alpine")
    {
        copyList<string>(alpineShaders, selectedBiomeShaderFiles);
        copyList<GLuint>(alpineShaderIDs, selectedBiomeShaderIDs);
    }
    else if (selectedBiome == "desert")
    {
        copyList<string>(desertShaders, selectedBiomeShaderFiles);
        copyList<GLuint>(desertShaderIDs, selectedBiomeShaderIDs);
    }
}

void TerrainClass::setBiome()
{
    selectedBiome = biomeNames[biomeIDX];

    setBiomeTextures();
    setBiomeShaders();

    numBiomeTextures = (int)selectedBiomeTextureIDs.size();
}

void TerrainClass::updateWind(double time_i)
{
    float amplitude = 10.0;
    windRotation = amplitude * sin(amplitude * time_i / 10.0);
}

void TerrainClass::initialize()
{
    readHeightMapFile();
    calculateNormals();
    readTrailsFile();
    setBiome();
}

void TerrainClass::generateNewHeightMap()
{
    PerlinModeON = true;
    initPerlinMap();
    calculateNormals();
}

void TerrainClass::initPerlinMap()
{
    // Reseed Perlin
    uint32_t newPerlinSeed = randomNumberGenerator();
    perlin.reseed(newPerlinSeed);

    heightMapMesh.clear();
    float noisyValue;

    float frequency = 2.5; // 0.1 to 64.0
    int octaves = 9; // 1 to 16

    int numRows = PerlinRows;
    int numCols = PerlinCols;
    cx = numRows / 2.0;
    cz = numCols / 2.0;
    float fx = numRows / frequency;
    float fy = numCols / frequency;

    vector<float> temp;
    numVertices = 0;
    for (int i=0; i < numRows; i++)
    {
        temp.clear();
        for (int j=0; j < numCols; j++)
        {
            noisyValue = (float)perlin.octaveNoise0_1(i / fx, j / fy, octaves) * 512 - 160;
            temp.push_back(noisyValue);
            numVertices++;
        }
        heightMapMesh.push_back(temp);
    }
}

void TerrainClass::updatePerlinMap()
{
    if (!PerlinModeON) { return; }
    // camX = cameraPosition.x + cx;
    // camZ = cameraPosition.z + cz;
    //
    // cx = camX;
    // cz = camZ;
    // cout << camX << ", " << camZ << endl;
    //
    // heightMapMesh.clear();
    // float noisyValue;
    //
    // float frequency = 2.5; // 0.1 to 64.0
    // int octaves = 9; // 1 to 16
    //
    // int numRows = PerlinRows;
    // int numCols = PerlinCols;
    // cx = numRows / 2.0;
    // cz = numCols / 2.0;
    // float fx = numRows / frequency;
    // float fy = numCols / frequency;
    //
    // vector<float> temp;
    // float m, n;
    // for (int i=0; i < numRows; i++)
    // {
    //     temp.clear();
    //     m = camX;
    //     for (int j=0; j < numCols; j++)
    //     {
    //         n = camZ;
    //         noisyValue = (float)perlin.octaveNoise0_1(m / fx, n / fy, octaves) * 512 - 160;
    //         temp.push_back(noisyValue);
    //         numVertices++;
    //     }
    //     heightMapMesh.push_back(temp);
    // }
}

void TerrainClass::readTrailsFile()
{
    string line;
    ifstream myfile(trailsFile);

    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            vector<float> row;
            float ele;
            istringstream inStream(line);
            while (inStream >> ele)
            {
                row.push_back(ele);
                numVertices++;
            }
            trailsMapMesh.push_back(row);
        }
        myfile.close();
    }
}

void TerrainClass::readHeightMapFile()
{
    // heightMapMesh.clear();
    string line;
    numVertices = 0;
    float ele;
    currentHeightMapFile = heightMaps[heightMapIDX];
    ifstream myfile(currentHeightMapFile);
    heightMapMesh.clear();
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            vector<float> row;
            istringstream inStream(line);
            while (inStream >> ele)
            {
                row.push_back(ele);
                numVertices++;
            }
            heightMapMesh.push_back(row);
        }
        myfile.close();
    }

    int nRows = (int) heightMapMesh.size();
    int nCols = (int) heightMapMesh[0].size();

    cx = nRows / 2.0;
    cz = nCols / 2.0;

    cout << "cx: " << cx << " -- cz: " << cz << endl;
    cout << "Num vertices: " << numVertices << endl;
}

void TerrainClass::cleanUpRender()
{
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
}

void TerrainClass::setShaders()
{
    GLuint shaderID = selectedBiomeShaderIDs[currentShaderID];
    glUseProgram(shaderID);

    GLuint maxPixelDistanceID = glGetUniformLocation(shaderID, "maxPixelDistance");
    glUniform1f(maxPixelDistanceID, maxPixelDistance);

    GLuint textureUniformID_0 = glGetUniformLocation(shaderID, "texture0");
    GLuint textureUniformID_1 = glGetUniformLocation(shaderID, "texture1");
    GLuint textureUniformID_2 = glGetUniformLocation(shaderID, "texture2");
    GLuint textureUniformID_3 = glGetUniformLocation(shaderID, "texture3");
    GLuint textureUniformID_4 = glGetUniformLocation(shaderID, "texture4");
    GLuint textureUniformID_5 = glGetUniformLocation(shaderID, "trailMap");

    float time = 0.001*glutGet(GLUT_ELAPSED_TIME);
    GLuint timeID = glGetUniformLocation(shaderID, "time");
    glUniform1f(timeID, time);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, selectedBiomeTextureIDs[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, selectedBiomeTextureIDs[1]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, selectedBiomeTextureIDs[2]);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, selectedBiomeTextureIDs[3]);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, selectedBiomeTextureIDs[4]);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, trailTextureMapIDs[0]);

    glUniform1i(textureUniformID_0, 0);
    glUniform1i(textureUniformID_1, 1);
    glUniform1i(textureUniformID_2, 2);
    glUniform1i(textureUniformID_3, 3);
    glUniform1i(textureUniformID_4, 4);
    glUniform1i(textureUniformID_5, 5);
}

void TerrainClass::setMaterials(string object)
{
    float shinynessValue, emissionValue, specularLight, R, G, B, A;
    if (object == "terrain")
    {
        //  Set materials
        shinynessValue = 10;
        emissionValue = 0.0;
        specularLight = 0.0;
        R = 1.0;
        G = 1.0;
        B = 1.0;
        A = 1.0;
    }
    else if (object == "grass")
    {
        //  Set materials
        shinynessValue = 0.2;
        emissionValue = 0.0;
        specularLight = 0.0;
        R = 1.0;
        G = 1.0;
        B = 1.0;
        A = 1.0;
    }
    else if (object == "tree_leaves")
    {
        //  Set materials
        shinynessValue = 1.0;
        emissionValue = 0.0;
        specularLight = 0.0;
        R = 1.0;
        G = 1.0;
        B = 1.0;
        A = 1.0;
    }
    else
    {
        //  Set materials
        shinynessValue = 16;
        emissionValue = 0.0;
        specularLight = 0.0;
        R = 1.0;
        G = 1.0;
        B = 1.0;
        A = 1.0;
    }

    float Shinyness[] = {shinynessValue};
    float RGBA[] = {R, G, B, A};
    float Specular[]  = {specularLight, specularLight, specularLight, 1.0};
    float Emission[]  = {emissionValue, emissionValue, emissionValue, 1.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Shinyness);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, RGBA);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RGBA);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
}

void TerrainClass::setTextures()
{
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


void TerrainClass::renderSolidTerrain()
{
    glm::vec3 v1, v2, v3, v4;
    glm::vec3 n1, n2, n3, n4;
    int i, j, m, n;

    int nRows = (int) heightMapMesh.size();
    int nCols = (int) heightMapMesh[0].size();

    glPushMatrix();
        glEnable(GL_CULL_FACE);
        setMaterials("terrain");
        setTextures();
        setShaders();

        glTranslatef(-cx, heightAdjust, -cz);

        glColor3f(1, 1, 1);
        for (i=0; i < nRows-1; i++)
        {
            m = i+1;
            for (j=0; j < nCols-1; j++)
            {
                n = j+1;
                glBegin(GL_QUADS);

                // Vertices
                v1 = glm::vec3(i,  readHeightMap(i, j)*heightScale,  j);
                v2 = glm::vec3(i,  readHeightMap(i, n)*heightScale,  n);
                v3 = glm::vec3(m,  readHeightMap(m, n)*heightScale,  n);
                v4 = glm::vec3(m,  readHeightMap(m, j)*heightScale,  j);

                // Normals
                n1 = normalMap[i][j];
                n2 = normalMap[i][n];
                n3 = normalMap[m][n];
                n4 = normalMap[m][j];

                // Vertex 1
                glNormal3f(n1.x, n1.y, n1.z);
                for (int i=0; i < numBiomeTextures; i++)
                {
                    glMultiTexCoord2f(GL_TEXTURE0 + i, 0.0, 0.0);
                }
                glMultiTexCoord2f(GL_TEXTURE5, (i)/(float)nRows, (j)/(float)nCols); // Trail map
                glVertex3f(v1.x, v1.y, v1.z);

                // Vertex 2
                glNormal3f(n2.x, n2.y, n2.z);
                for (int i=0; i < numBiomeTextures; i++)
                {
                    glMultiTexCoord2f(GL_TEXTURE0 + i, 0.0, 1.0);
                }
                glMultiTexCoord2f(GL_TEXTURE5, (i)/(float)nRows, (j+1)/(float)nCols); // Trail map
                glVertex3f(v2.x, v2.y, v2.z);

                // Vertex 3
                glNormal3f(n3.x, n3.y, n3.z);
                for (int i=0; i < numBiomeTextures; i++)
                {
                    glMultiTexCoord2f(GL_TEXTURE0 + i, 1.0, 1.0);
                }
                glMultiTexCoord2f(GL_TEXTURE5, (i+1)/(float)nRows, (j+1)/(float)nCols); // Trail map
                glVertex3f(v3.x, v3.y, v3.z);

                // Vertex 4
                glNormal3f(n4.x, n4.y, n4.z);
                for (int i=0; i < numBiomeTextures; i++)
                {
                    glMultiTexCoord2f(GL_TEXTURE0 + i, 1.0, 0.0);
                }
                glMultiTexCoord2f(GL_TEXTURE5, (i+1)/(float)nRows, (j)/(float)nCols); // Trail map
                glVertex3f(v4.x, v4.y, v4.z);
                glEnd();
            }
        }
        glDisable(GL_CULL_FACE);
    glPopMatrix();
    cleanUpRender();
}

void TerrainClass::calculateNormals()
{
    normalMap.clear();
    glm::vec3 v0, vTop, vBottom, vLeft, vRight;
    glm::vec3 n0, nTop, nBottom, nLeft, nRight;
    glm::vec3 v1, v2, v3, v4;

    int nRows = (int) heightMapMesh.size();
    int nCols = (int) heightMapMesh[0].size();

    vector<glm::vec3> tempRow;
    for (int i=0; i < nRows; i++)
    {
        tempRow.clear();

        for (int j=0; j < nCols; j++)
        {
            if (i == 0 || i == nRows-1 || j == 0 || j == nCols-1)
            {
                tempRow.push_back(glm::vec3(0, 1, 0));
            }
            else
            {
                v0 = glm::vec3(i,  readHeightMap(i, j)*heightScale,  j);
                vTop    = glm::vec3(i,     readHeightMap(i, j+1)*heightScale,  j+1);
                vBottom = glm::vec3(i,     readHeightMap(i, j-1)*heightScale,  j-1);
                vLeft   = glm::vec3(i-1,   readHeightMap(i-1, j)*heightScale,  j);
                vRight  = glm::vec3(i+1,   readHeightMap(i+1, j)*heightScale,  j);

                v1 = -glm::cross(vTop - v0, vLeft - v0);
                v2 = -glm::cross(vLeft - v0, vBottom - v0);
                v3 = -glm::cross(vBottom - v0, vRight - v0);
                v4 = -glm::cross(vRight - v0, vTop - v0);

                float xAve, yAve, zAve;
                xAve = (v1.x + v2.x + v3.x + v4.x) / 4.0;
                yAve = (v1.y + v2.y + v3.y + v4.y) / 4.0;
                zAve = (v1.z + v2.z + v3.z + v4.z) / 4.0;
                glm::vec3 normal = glm::normalize(glm::vec3(xAve, yAve, zAve));

                tempRow.push_back(normal);
            }
        }
        normalMap.push_back(tempRow);
    }
}

void TerrainClass::renderNormals()
{
    glm::vec3 v1, n1;
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glLineWidth(4.0f);
    glColor3f(1, 0, 0);
    glPushMatrix();
        glTranslatef(-cx, heightAdjust, -cz);
        int nRows = (int) heightMapMesh.size();
        int nCols = (int) heightMapMesh[0].size();
        for (int i=0; i < nRows; i++)
        {
            for (int j=0; j < nCols; j++)
            {
                v1 = glm::vec3(i,  readHeightMap(i, j)*heightScale,  j);
                n1 = normalMap[i][j];
                // Line
                glBegin(GL_LINES);
                glVertex3f(v1.x, v1.y, v1.z);
                glVertex3f(v1.x+n1.x, v1.y+n1.y, v1.z+n1.z);

                glEnd();
            }
        }
    glPopMatrix();
    glLineWidth(1.f); // Reset line widths

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}

void TerrainClass::renderMeshWire()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();

        glTranslatef(-cx, heightAdjust, -cz);
        glColor3f(0, 0, 0);

        int nRows = (int) heightMapMesh.size();
        int nCols = (int) heightMapMesh[0].size();

        for (int i=0; i < nRows; i++)
        {
            glBegin(GL_LINE_STRIP);
            for (int j=0; j < nCols; j++)
            {
                glVertex3d(i, readHeightMap(i, j)*heightScale, j);
            }
            glEnd();
        }
        for (int i=0; i < nRows; i++)
        {
            glBegin(GL_LINE_STRIP);
            for (int j=0; j < nCols; j++)
            {
                glVertex3d(j, readHeightMap(j, i)*heightScale, i);
            }
            glEnd();
        }
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

float TerrainClass::readHeightMap(int i, int j)
{
    return heightMapMesh[i][j];
}

void TerrainClass::renderTreesOrCacti(int i, int j)
{
    setMaterials("tree_leaves");
    if (readHeightMap(i, j)*heightScale > maxTreeAltitude || readHeightMap(i, j)*heightScale < minTreeAltitude)
    {
        return;
    }

    if (!(i % 10 == 0 && j % 10 == 0))
    {
        return;
    }

    glm::vec3 v1 = glm::vec3(i,  readHeightMap(i, j)*heightScale,  j);

    // Render Tree or Cacti
    // -----------------
    if (selectedBiome == "alpine")
    {
        trees.render(i, j, v1, windRotation);
    }
    else if (selectedBiome == "desert")
    {
        cacti.render(i, j, v1, windRotation);
    }
    // -----------------
}

void TerrainClass::renderGrassBlade(int i, int j, glm::vec3 v1, float rotation)
{
    float grassScale = 0.25;
    glPushMatrix();
        glTranslatef(v1.x, v1.y + grassScale - 0.05, v1.z);
        glRotatef(rotation, 0, 1, 0);
        glRotatef(windRotation, 1, 0, 0);
        glScalef(grassScale, grassScale, grassScale);
        glBegin(GL_QUADS);

        float normDirection = -1;
        glNormal3f(0, 0, normDirection);
        glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0); glVertex3f(-1, -1, 0);
        glNormal3f(0, 0, normDirection);
        glMultiTexCoord2f(GL_TEXTURE0, 0.0, 1.0); glVertex3f(-1, 1, 0);
        glNormal3f(0, 0, normDirection);
        glMultiTexCoord2f(GL_TEXTURE0, 1.0, 1.0); glVertex3f(1, 1, 0);
        glNormal3f(0, 0, normDirection);
        glMultiTexCoord2f(GL_TEXTURE0, 1.0, 0.0); glVertex3f(1, -1, 0);
        glEnd();
    glPopMatrix();
}

void TerrainClass::renderGrass(int i, int j, float distance)
{
    glDisable(GL_LIGHTING);
    setMaterials("grass");
    float multiBillboardDistanceThreshold = 20;
    float trainThreshold = 50;

    float sep_i, sep_j, row_i, col_j;
    if (readHeightMap(i, j)*heightScale > maxGrassAltitude || readHeightMap(i, j)*heightScale < minGrassAltitude)
    {
        return;
    }

    GLuint shaderID = grassShaderProgramIDs[0];
    glUseProgram(shaderID);

    GLuint billboardID = glGetUniformLocation(shaderID, "texture");

    float time = 0.001*glutGet(GLUT_ELAPSED_TIME);
    GLuint timeID = glGetUniformLocation(shaderID, "time");
    glUniform1f(timeID, time);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, selectedBiomeVegetationTextureIDs[3]);

    glUniform1i(billboardID, 0);

    glm::vec3 v1 = glm::vec3(i,  readHeightMap(i, j)*heightScale,  j);
    if (trailsMapMesh[i][j] >= trainThreshold)
    {
        if (distance < multiBillboardDistanceThreshold)
        {
            renderGrassBlade(i, j, v1, 0);
            renderGrassBlade(i, j, v1, 60);
            renderGrassBlade(i, j, v1, 120);
        }
        else
        {
            renderGrassBlade(i, j, v1, 0);
            renderGrassBlade(i, j, v1, 90);
        }
    }

    vector<float> rowPositions = {-2, -0.5, 1, 1.5, 2};
    vector<float> colPositions = {-2, -1.5, 1, 0.5, 1.83};

    for (int m=0; m < (int)rowPositions.size(); m++)
    {
        row_i = rowPositions[m];
        sep_i = i + row_i*grassSeparation;
        for (int n=0; n < (int)colPositions.size(); n++)
        {
            if (m % 2 == 0)
            {
                glBindTexture(GL_TEXTURE_2D, selectedBiomeVegetationTextureIDs[1]);
                glUniform1i(billboardID, 0);
            }
            else if (m % 3 == 0)
            {
                glBindTexture(GL_TEXTURE_2D, selectedBiomeVegetationTextureIDs[3]);
                glUniform1i(billboardID, 0);
            }
            else if (n % 2 == 1)
            {
                glBindTexture(GL_TEXTURE_2D, selectedBiomeVegetationTextureIDs[2]);
                glUniform1i(billboardID, 0);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, selectedBiomeVegetationTextureIDs[4]);
                glUniform1i(billboardID, 0);
            }
            col_j = colPositions[n];
            sep_j = j + col_j*grassSeparation;

            if (trailsMapMesh[sep_i][sep_j] < trainThreshold)
            {
                continue;
            }

            v1 = glm::vec3(sep_i,  readHeightMap((int)sep_i, (int)sep_j)*heightScale,  sep_j);

            if (distance < multiBillboardDistanceThreshold)
            {
                renderGrassBlade(i, j, v1, 0);
                renderGrassBlade(i, j, v1, 60);
                renderGrassBlade(i, j, v1, 120);
            }
            else
            {
                renderGrassBlade(i, j, v1, 0);
                renderGrassBlade(i, j, v1, 90);
            }
        }
    }
    glEnable(GL_LIGHTING);
}

void TerrainClass::renderVegetation()
{
    glPushMatrix();
        setTextures();
        glTranslatef(-cx, heightAdjust, -cz);

        int start_row_i = (int) cameraPosition.x - grassRenderDistance;
        int end_row_i = (int) cameraPosition.x + grassRenderDistance;
        int start_row_j = (int) cameraPosition.z - grassRenderDistance;
        int end_row_j = (int) cameraPosition.z + grassRenderDistance;

        int nRows = (int) heightMapMesh.size();
        int nCols = (int) heightMapMesh[0].size();

        glm::vec2 renderPos, diff;

        float currentDistanceFromCamera;
        for (int i=start_row_i; i < end_row_i; i++)
        {
            for (int j=start_row_j; j < end_row_j; j++)
            {
                renderPos = glm::vec2(i, j);
                diff = renderPos - glm::vec2(cameraPosition.x, cameraPosition.z);
                currentDistanceFromCamera = glm::dot(diff, diff);
                if (i < 0 || j < 0 || i > nRows-1 || j > nCols-1)
                {
                    continue;
                }
                renderGrass(i, j, currentDistanceFromCamera);
                renderTreesOrCacti(i, j);
            }
        }
    glPopMatrix();
    cleanUpRender();
}

float TerrainClass::getHeight()
{
    float x, y, z;
    camX = cameraPosition.x + cx;
    camZ = cameraPosition.z + cz;

    x = (int) camX;
    z = (int) camZ;
    int nRows = (int) heightMapMesh.size();
    int nCols = (int) heightMapMesh[0].size();

    if (x < 0 || x > nRows-1 || z == 0 || z > nCols-1)
    {
        y = 0;
    }
    else
    {
        y = (readHeightMap(x, z)*heightScale) + heightAdjust;
    }
    cameraPosition.x = camX;
    cameraPosition.y = y;
    cameraPosition.z = camZ;
    return y;
}

void TerrainClass::render()
{
    glDisable(GL_BLEND); // Ensure blend is disabled
    if (showFullTerrain)
    {
        renderSolidTerrain();
    }

    if (showMeshWire)
    {
        renderMeshWire();
    }

    if (showNormals)
    {
        renderNormals();
    }

    if (showVegetation)
    {
        renderVegetation();
    }
}

void TerrainClass::toggleMeshWire()
{
    showMeshWire = !showMeshWire;
}

void TerrainClass::toggleSolidTerrain()
{
    showFullTerrain = !showFullTerrain;
}

void TerrainClass::setCameraPosition(glm::vec3 newCameraPosition)
{
    cameraPosition = newCameraPosition;
    updatePerlinMap();
}

void TerrainClass::toggleShowNormals()
{
    showNormals = !showNormals;
}

string TerrainClass::getShaderText()
{
    return selectedBiomeShaderFiles[currentShaderID];
}

void TerrainClass::toggleShader()
{
    currentShaderID++;
    currentShaderID %= (int)selectedBiomeShaderIDs.size();
}

void TerrainClass::toggleBiome()
{
    biomeIDX++;
    biomeIDX %= (int)biomeNames.size();
    setBiome();
}

void TerrainClass::toggleVegetation()
{
    showVegetation = !showVegetation;
}

void TerrainClass::toggleTextureColoring()
{
    showTextureColorOnly = !showTextureColorOnly;

    if (showTextureColorOnly)
    {
        copyList<GLuint>(selectedBiomeTextureIDs, selectedBiomeTextureIDs_backup);
        selectedBiomeTextureIDs.clear();
    }
    else
    {
        copyList<GLuint>(selectedBiomeTextureIDs_backup, selectedBiomeTextureIDs);
    }
    setBiome();
}

void TerrainClass::toggleHeightMap()
{
    heightMapIDX++;
    heightMapIDX %= (int) heightMaps.size();

    readHeightMapFile();
    PerlinModeON = false;
}

void TerrainClass::lowerHeight()
{
    int nRows = (int) heightMapMesh.size();
    int nCols = (int) heightMapMesh[0].size();
    for (int i=0; i < nRows; i++)
    {
        for (int j=0; j < nCols; j++)
        {
            heightMapMesh[i][j] -= deltaHeight;
        }
    }
}

void TerrainClass::raiseHeight()
{
    int nRows = (int) heightMapMesh.size();
    int nCols = (int) heightMapMesh[0].size();
    for (int i=0; i < nRows; i++)
    {
        for (int j=0; j < nCols; j++)
        {
            heightMapMesh[i][j] += deltaHeight;
        }
    }
}

void TerrainClass::increaseRenderDistance()
{
    maxPixelDistance += 15;
}

void TerrainClass::decreaseRenderDistance()
{
    maxPixelDistance -= 15;
}
