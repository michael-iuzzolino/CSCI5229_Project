// TextureHandlerClass.h
#ifndef _TextureHandlerClass_H
#define _TextureHandlerClass_H
#include "projectManager.h"

class TextureHandlerClass
{
public:
    int numTextures;
    map <string, GLuint> IDlookup;

    TextureHandlerClass();
    void setTextures(vector<string>, vector<GLuint>&, bool flipImage=true);
    void printLookup();
    void loadCubeMapTexture(vector<string>, GLuint&);
private:
    GLuint loadTextureFromFile(string, string, bool);
};
#endif
