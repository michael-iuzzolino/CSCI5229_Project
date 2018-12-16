//  Identity vertex shader
attribute vec3 inPosition;

varying vec3 texCoords;

void main()
{
    texCoords = inPosition;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(inPosition.xyz, 1.0);
}
