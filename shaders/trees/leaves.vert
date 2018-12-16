//  Phong lighting

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;

uniform float time;

void main()
{
    //  Vertex location in modelview coordinates
    vec4 P = gl_ModelViewMatrix * gl_Vertex;

    //  Light position
    Light  = gl_LightSource[0].position.xyz - P.xyz;

    //  Normal
    Normal = gl_NormalMatrix * gl_Normal;

    //  Eye position
    View  = -P.xyz;

    // Textures
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_MultiTexCoord1;
    gl_TexCoord[2] = gl_MultiTexCoord2;

    //  Set vertex coordinates
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex + vec4(sin(time)*0.01, sin(time)*0.01 + cos(time)*0.01, 0, 0);

}
