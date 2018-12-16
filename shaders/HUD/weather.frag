uniform float time;

void main()
{
    float timeFactor = (sin(time*10) + 1)*0.5;
    gl_FragColor = vec4(vec3(gl_Color), timeFactor);
}
