// TextureHandlerClass.h
#ifndef _TextureHandlerClass_H
#define _TextureHandlerClass_H
#include "projectManager.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <iterator>

#include "stb_image.h"

using namespace std;

class TextureHandlerClass
{
public:
    int numTextures;
    map <string, GLuint> IDlookup;

    TextureHandlerClass();
    void setTextures(vector<string>, vector<GLuint>&, bool flipImage=true);
    void printLookup();
    GLuint loadCubeMapTexture(vector<string>);
private:
    GLuint loadTextureFromFile(string, string, bool);
};
#endif
