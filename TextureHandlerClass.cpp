// TextureHandlerClass.cpp
#include "TextureHandlerClass.h"

TextureHandlerClass::TextureHandlerClass() {}

void TextureHandlerClass::setTextures(vector<string> texturePaths, vector<GLuint>& textureIDs, bool flipImage)
{
    GLuint textureID;
    string textureFile;
    string imagePath, imageType;
    textureIDs.clear();

    for (int i=0; i < (int)texturePaths.size(); i+=2)
    {
        cout << i << "/" << (int)texturePaths.size() << " files loaded..." << endl;
        imagePath = texturePaths[i];
        imageType = texturePaths[i+1];

        textureFile = "textures/" + imagePath;
        textureID = loadTextureFromFile(textureFile, imageType, flipImage);
        textureIDs.push_back(textureID);

        // Update Lookup
        IDlookup.insert(pair <string, GLuint> (textureFile, textureID));
    }
    numTextures = (int) textureIDs.size();
}

void TextureHandlerClass::printLookup()
{
    cout << "\nTextures" << endl;
    cout << "========" << endl;
    map <string, GLuint> :: iterator itr;
    int i = 1;
    for (itr=IDlookup.begin(); itr != IDlookup.end(); ++itr)
    {
        cout << i << ". " << itr->first << " -- ID: " << itr->second << endl;
        i++;
    }
}

void TextureHandlerClass::loadCubeMapTexture(vector<string> faces, GLuint& textureID)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            cout << "Cubemap texture failed to load at path: " << faces[i] << endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

GLuint TextureHandlerClass::loadTextureFromFile(string filename, string imgType, bool flipImage)
{
    GLuint texture;
    GLuint imgFormat = GL_RGB;
    GLuint stbiFormat = STBI_rgb;
    int width, height, nChannels;

    cout << "Loading File: " << filename <<  endl;

    if (imgType == "RGB")
    {
        imgFormat = GL_RGB;
        stbiFormat = STBI_rgb;
    }
    else if (imgType == "RGBA")
    {
        imgFormat = GL_RGBA;
        stbiFormat = STBI_rgb_alpha;
    }
    else if (imgType == "ALPHA")
    {
        imgFormat = GL_ALPHA;
        stbiFormat = STBI_rgb_alpha;
    }

    stbi_set_flip_vertically_on_load(flipImage);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nChannels, stbiFormat);
    cout << "\t" << filename << " has " << nChannels << " channels." << endl;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, width, height, 0, imgFormat, GL_UNSIGNED_BYTE, data);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    cout << "\tSuccessfully loaded." << endl;
    return texture;
}
