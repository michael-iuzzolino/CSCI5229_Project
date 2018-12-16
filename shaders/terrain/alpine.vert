//  Phong lighting

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;
varying vec3 vertexNorm;
varying float distToCamera;
varying float height;

void main()
{
    //  Vertex location in modelview coordinates
    vec4 P = gl_ModelViewMatrix * gl_Vertex;

    distToCamera = -P.z;

    //  Light position
    Light  = gl_LightSource[0].position.xyz - P.xyz;

    //  Normal
    Normal = gl_NormalMatrix * gl_Normal;

    //  Eye position
    View  = -P.xyz;

    height = gl_Vertex.y;
    vertexNorm = gl_Normal;

    // Texture
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_MultiTexCoord1;
    gl_TexCoord[2] = gl_MultiTexCoord2;
    gl_TexCoord[3] = gl_MultiTexCoord3;
    gl_TexCoord[4] = gl_MultiTexCoord4;
    gl_TexCoord[5] = gl_MultiTexCoord5;

    //  Return fixed transform coordinates for this vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
