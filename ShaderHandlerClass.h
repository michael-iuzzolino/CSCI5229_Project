// ShaderHandlerClass.h
#ifndef _ShaderHandlerClass_H
#define _ShaderHandlerClass_H
#include "projectManager.h"

class ShaderHandlerClass
{
public:
    int numShaders;
    map <string, GLuint> IDlookup;

    ShaderHandlerClass();
    void setShaders(vector<string>, vector<GLuint>&);
    void printLookup();
private:
    string readText(string);
    int createShader(GLenum, string);
    int createShaderProg(string, string);
};
#endif
