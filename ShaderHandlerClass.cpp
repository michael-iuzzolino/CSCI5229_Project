// ShaderHandlerClass.cpp
#include "ShaderHandlerClass.h"

ShaderHandlerClass::ShaderHandlerClass() {}

string ShaderHandlerClass::readText(string filename)
{
    string line, allLines;
    allLines = "";
    ifstream theFile(filename);
    if (theFile.is_open())
    {
        while (getline(theFile, line))
        {
            allLines = allLines + line + "\n";
        }
        theFile.close();
    }
    else
    {
        cout << "Unable to open file: " << filename << endl;
    }
    return allLines;
}

int ShaderHandlerClass::createShader(GLenum type, string file)
{
   //  Create the shader
   int shaderID = glCreateShader(type);
   string source = readText(file);
   //  Load source code from file
   const char *c_str = source.c_str(); //https://stackoverflow.com/questions/6047527/how-to-pass-an-stdstring-to-glshadersource
   glShaderSource(shaderID, 1, &c_str, NULL);
   //  Compile the shader
   cout << "Compile: " << file << endl;
   glCompileShader(shaderID);

   //  Return name
   return shaderID;
}

int ShaderHandlerClass::createShaderProg(string vertFile, string fragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = createShader(GL_VERTEX_SHADER, vertFile);
   //  Create and compile fragment shader
   int frag = createShader(GL_FRAGMENT_SHADER, fragFile);
   //  Attach vertex shader
   glAttachShader(prog, vert);
   //  Attach fragment shader
   glAttachShader(prog, frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors

   return prog;
}

void ShaderHandlerClass::setShaders(vector<string> shaderFiles, vector<GLuint>& shaderProgramIDs)
{
    int shaderID;
    string vertexShader, fragmentShader;

    shaderProgramIDs.clear();

    for (int i=0; i < (int)shaderFiles.size(); i+=2)
    {
        vertexShader = "shaders/" + shaderFiles[i];
        fragmentShader = "shaders/" + shaderFiles[i+1];
        shaderID = createShaderProg(vertexShader, fragmentShader);
        shaderProgramIDs.push_back(shaderID);

        // Update Lookup
        IDlookup.insert(pair <string, GLuint> (shaderFiles[i], shaderID));
    }

    numShaders = (int) shaderProgramIDs.size();
}

void ShaderHandlerClass::printLookup()
{
    cout << "\nShaders" << endl;
    cout << "=======" << endl;
    map <string, GLuint> :: iterator itr;
    int i = 1;
    for (itr=IDlookup.begin(); itr != IDlookup.end(); ++itr)
    {
        cout << i << ". " << itr->first << " -- ID: " << itr->second << endl;
        i++;
    }
}
