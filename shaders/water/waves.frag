// Author @patriciogv - 2015
// http://patriciogonzalezvivo.com

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform int textureNum;

varying float LightIntensity;

void main()
{
    vec2 texCoord0 = gl_TexCoord[0].xy;
    vec2 texCoord1 = gl_TexCoord[1].xy;
    vec4 color;

    color = vec4(0.5, 0.5, 0.5, 0.8);

    if (textureNum == 0)
    {
        gl_FragColor = color * texture2D(texture0, texCoord0.st);
    }
    else
    {
        gl_FragColor = color * texture2D(texture1, texCoord1.st);
    }
}
