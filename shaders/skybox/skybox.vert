attribute vec3 inPosition;

verying texCoord;

void main()
{
    texCoord = normalize(inPosition.xyz);

    gl_Position = gl_ModelViewProjectionMatrix * vec4(inPosition, 1.0);
}
