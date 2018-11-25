uniform samplerCube skybox;

varying texCoord;

void main()
{
    // gl_FragColor = texture(skybox, texCoord);
    gl_FragColor = vec4(1, 0, 0, 1);
}
