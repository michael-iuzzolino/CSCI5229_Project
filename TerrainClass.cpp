// TerrainClass.cpp
#include "TerrainClass.h"

TerrainClass::TerrainClass()
{
    // INITIAL VALUES
    // ======================
    biomeIDX = 0;
    currentShaderID = 0;

    heightScale = 0.165;
    heightAdjust = -10;

    grassRenderDistance = 15;
    maxGrassAltitude = 10;
    minGrassAltitude = 4.5;
    grassSeparation = 1.75;
    // ======================

    // FLAGS
    // ======================
    showFullTerrain = true;
    showMeshWire = true;
    showVegetation = true;
    showNormals = true;
    showTextureColorOnly = false;
    // ======================
}

void TerrainClass::clearCurrentBiome()
{
    for (int i=0; i < (int) selectedBiomeTextureIDs.size(); i++)
    {
        glDeleteTextures(1, &selectedBiomeTextureIDs[i]);
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

void TerrainClass::setBiome()
{
    selectedBiome = biomeNames[biomeIDX];

    if (showTextureColorOnly)
    {
        copyList<GLuint>(selectedBiomeTextureIDs, selectedBiomeTextureIDs_backup);
        selectedBiomeTextureIDs.clear();
    }
    else
    {
        copyList<GLuint>(selectedBiomeTextureIDs_backup, selectedBiomeTextureIDs);

    }
    selectedBiomeShaderIDs.clear();
    selectedBiomeTextureFiles.clear();
    selectedBiomeShaderFiles.clear();

    if (selectedBiome == "alpine")
    {
        copyList<string>(alpineTextureFiles, selectedBiomeTextureFiles);
        copyList<string>(alpineShaders, selectedBiomeShaderFiles);
        copyList<GLuint>(alpineShaderIDs, selectedBiomeShaderIDs);
    }
    else if (selectedBiome == "desert")
    {
        copyList<string>(desertTextureFiles, selectedBiomeTextureFiles);
        copyList<string>(desertShaders, selectedBiomeShaderFiles);
        copyList<GLuint>(desertShaderIDs, selectedBiomeShaderIDs);
    }

    numBiomeTextures = (int)selectedBiomeTextureIDs.size();
}

void TerrainClass::updateWind(double time_i)
{
    float amplitude = 10.0;
    grassWindRotation = amplitude * sin(amplitude * time_i / 10.0);
}

void TerrainClass::initialize()
{
    readHeightMapFile();
    calculateNormals();
    readTrailsFile();
    setBiome();
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
            numCols = (int)row.size();
        }
        myfile.close();
    }
}
void TerrainClass::readHeightMapFile()
{
    string line;
    ifstream myfile(heightMapFile);
    numVertices = 0;
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
            heightMapMesh.push_back(row);
            numCols = (int)row.size();
        }
        myfile.close();
    }

    numRows = (int)heightMapMesh.size();
    cx = numRows / 2.0;
    cz = numCols / 2.0;

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

    GLuint textureUniformID_0 = glGetUniformLocation(shaderID, "texture0");
    GLuint textureUniformID_1 = glGetUniformLocation(shaderID, "texture1");
    GLuint textureUniformID_2 = glGetUniformLocation(shaderID, "texture2");
    GLuint textureUniformID_3 = glGetUniformLocation(shaderID, "texture3");
    GLuint textureUniformID_4 = glGetUniformLocation(shaderID, "texture4");
    GLuint textureUniformID_5 = glGetUniformLocation(shaderID, "trailMap");

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
        shinynessValue = 2;
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

    glPushMatrix();
    glEnable(GL_CULL_FACE);
    setMaterials("terrain");
    setTextures();
    setShaders();

    glTranslatef(-cx, heightAdjust, -cz);

    glColor3f(1, 1, 1);
    for (i=0; i < numRows-1; i++)
    {
        m = i+1;
        for (j=0; j < numCols-1; j++)
        {
            n = j+1;
            glBegin(GL_QUADS);

            // Vertices
            v1 = glm::vec3(i,  heightMapMesh[i][j]*heightScale,  j);
            v2 = glm::vec3(i,  heightMapMesh[i][n]*heightScale,  n);
            v3 = glm::vec3(m,  heightMapMesh[m][n]*heightScale,  n);
            v4 = glm::vec3(m,  heightMapMesh[m][j]*heightScale,  j);

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
            glMultiTexCoord2f(GL_TEXTURE5, (i)/(float)numRows, (j)/(float)numCols); // Trail map
            glVertex3f(v1.x, v1.y, v1.z);

            // Vertex 2
            glNormal3f(n2.x, n2.y, n2.z);
            for (int i=0; i < numBiomeTextures; i++)
            {
                glMultiTexCoord2f(GL_TEXTURE0 + i, 0.0, 1.0);
            }
            glMultiTexCoord2f(GL_TEXTURE5, (i)/(float)numRows, (j+1)/(float)numCols); // Trail map
            glVertex3f(v2.x, v2.y, v2.z);

            // Vertex 3
            glNormal3f(n3.x, n3.y, n3.z);
            for (int i=0; i < numBiomeTextures; i++)
            {
                glMultiTexCoord2f(GL_TEXTURE0 + i, 1.0, 1.0);
            }
            glMultiTexCoord2f(GL_TEXTURE5, (i+1)/(float)numRows, (j+1)/(float)numCols); // Trail map
            glVertex3f(v3.x, v3.y, v3.z);

            // Vertex 4
            glNormal3f(n4.x, n4.y, n4.z);
            for (int i=0; i < numBiomeTextures; i++)
            {
                glMultiTexCoord2f(GL_TEXTURE0 + i, 1.0, 0.0);
            }
            glMultiTexCoord2f(GL_TEXTURE5, (i+1)/(float)numRows, (j)/(float)numCols); // Trail map
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
    glm::vec3 v0, vTop, vBottom, vLeft, vRight;
    glm::vec3 n0, nTop, nBottom, nLeft, nRight;

    vector<glm::vec3> tempRow;
    for (int i=0; i < numRows; i++)
    {
        tempRow.clear();

        for (int j=0; j < numCols; j++)
        {

            if (i == 0 || i == numRows-1 || j == 0 || j == numCols-1)
            {
                tempRow.push_back(glm::vec3(0, 1, 0));
                continue;
            }

            // Vertices
            v0 = glm::vec3(i,  heightMapMesh[i][j]*heightScale,  j);

            vTop    = glm::vec3(i, heightMapMesh[i][j+1]*heightScale,  j+1);
            vBottom = glm::vec3(i, heightMapMesh[i][j-1]*heightScale,  j-1);
            vLeft   = glm::vec3(i-1,   heightMapMesh[i-1][j]*heightScale,  j);
            vRight  = glm::vec3(i+1,   heightMapMesh[i+1][j]*heightScale,  j);

            glm::vec3 normal = glm::normalize(glm::cross(vTop - vBottom, vRight - vLeft));
            tempRow.push_back(normal);
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
    for (int i=0; i < numRows; i++)
    {
        for (int j=0; j < numCols; j++)
        {
            v1 = glm::vec3(i,  heightMapMesh[i][j]*heightScale,  j);
            n1 = normalMap[i][j];
            // Line
            glBegin(GL_LINES);
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v1.x+n1.x, v1.y+n1.y, v1.z+n1.y);

            glEnd();
        }
    }
    glPopMatrix();
    glLineWidth(1.f); // Reset line widths
}

void TerrainClass::renderMeshWire()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();

    glTranslatef(-cx, heightAdjust, -cz);

    glColor3f(0, 0, 0);

    for (int i=0; i < numRows; i++)
    {
        glBegin(GL_LINE_STRIP);
        for (int j=0; j < numCols; j++)
        {
            glVertex3d(i, heightMapMesh[i][j]*heightScale, j);
        }
        glEnd();
    }
    for (int i=0; i < numRows; i++)
    {
        glBegin(GL_LINE_STRIP);
        for (int j=0; j < numCols; j++)
        {
            glVertex3d(j, heightMapMesh[j][i]*heightScale, i);
        }
        glEnd();
    }
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void TerrainClass::renderGrassBlade(int i, int j, glm::vec3 v1, float rotation)
{
    float grassScale = 0.25;
    glPushMatrix();
    glTranslatef(v1.x, v1.y + grassScale - 0.05, v1.z);
    glRotatef(rotation, 0, 1, 0);
    glRotatef(grassWindRotation, 1, 0, 0);
    glScalef(grassScale, grassScale, grassScale);
    glBegin(GL_QUADS);
    glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0); glVertex3f(-1, -1, 0);
    glMultiTexCoord2f(GL_TEXTURE0, 1.0, 0.0); glVertex3f(1, -1, 0);
    glMultiTexCoord2f(GL_TEXTURE0, 1.0, 1.0); glVertex3f(1, 1, 0);
    glMultiTexCoord2f(GL_TEXTURE0, 0.0, 1.0); glVertex3f(-1, 1, 0);
    glEnd();
    glPopMatrix();
}

void TerrainClass::renderGrassPatch(int i, int j)
{
    float trainThreshold = 50;

    float sep_i, sep_j, row_i, col_j;
    if (heightMapMesh[i][j]*heightScale > maxGrassAltitude || heightMapMesh[i][j]*heightScale < minGrassAltitude)
    {
        return;
    }

    GLuint shaderID = grassShaderProgramIDs[0];
    glUseProgram(shaderID);

    GLuint billboardID = glGetUniformLocation(shaderID, "texture");
    GLuint timeID = glGetUniformLocation(shaderID, "time");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassBillboardIDs[3]);

    glUniform1i(billboardID, 0);
    glUniform1i(timeID, glutGet(GLUT_ELAPSED_TIME)/1000.0);

    glm::vec3 v1 = glm::vec3(i,  heightMapMesh[i][j]*heightScale,  j);
    if (trailsMapMesh[i][j] >= trainThreshold)
    {
        renderGrassBlade(i, j, v1, 0);
        renderGrassBlade(i, j, v1, 60);
        renderGrassBlade(i, j, v1, 120);
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
                glBindTexture(GL_TEXTURE_2D, grassBillboardIDs[1]);
                glUniform1i(billboardID, 0);
            }
            else if (m % 3 == 0)
            {
                glBindTexture(GL_TEXTURE_2D, grassBillboardIDs[3]);
                glUniform1i(billboardID, 0);
            }
            else if (n % 2 == 1)
            {
                glBindTexture(GL_TEXTURE_2D, grassBillboardIDs[2]);
                glUniform1i(billboardID, 0);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, grassBillboardIDs[4]);
                glUniform1i(billboardID, 0);
            }
            col_j = colPositions[n];
            sep_j = j + col_j*grassSeparation;

            if (trailsMapMesh[sep_i][sep_j] < trainThreshold)
            {
                continue;
            }

            v1 = glm::vec3(sep_i,  heightMapMesh[(int)sep_i][(int)sep_j]*heightScale,  sep_j);
            renderGrassBlade(i, j, v1, 0);
            renderGrassBlade(i, j, v1, 60);
            renderGrassBlade(i, j, v1, 120);
        }
    }
}

void TerrainClass::renderGrass()
{
    glPushMatrix();
    setMaterials("grass");
    setTextures();
    glTranslatef(-cx, heightAdjust, -cz);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int start_row_i = (int) cameraPosition.x - grassRenderDistance;
    int end_row_i = (int) cameraPosition.x + grassRenderDistance;
    int start_row_j = (int) cameraPosition.z - grassRenderDistance;
    int end_row_j = (int) cameraPosition.z + grassRenderDistance;

    for (int i=start_row_i; i < end_row_i; i++)
    {
        for (int j=start_row_j; j < end_row_j; j++)
        {
            if (i < 0 || j < 0 || i > numRows-1 || j > numCols-1)
            {
                continue;
            }
            renderGrassPatch(i, j);
        }
    }
    glPopMatrix();
    cleanUpRender();
}

float TerrainClass::getHeight()
{
    float x, y, z;
    float camX = cameraPosition.x + cx;
    float camZ = cameraPosition.z + cz;

    x = (int) camX;
    z = (int) camZ;

    if (x < 0 || x > numRows-1 || z == 0 || z > numCols-1)
    {
        y = 0;
    }
    else
    {
        y = (heightMapMesh[x][z]*heightScale) + heightAdjust;
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
        renderGrass();
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
    setBiome();
}
