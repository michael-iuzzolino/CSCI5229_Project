uniform samplerCube skybox;

varying vec3 texCoords;

void main()
{
    gl_FragColor = texture(skybox, texCoords);
}
