// Author @patriciogv - 2015
// http://patriciogonzalezvivo.com


uniform sampler2D texture0;
uniform sampler2D texture1;
uniform bool reflectionON;
uniform samplerCube skybox;

uniform float worldRotationRate;
uniform float time;
uniform int textureNum;

varying float LightIntensity;
varying vec3 Position;
varying vec3 Normal;


mat3 rotationMatrix33(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat3(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c);
}

mat4 rotationMatrix44(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
    vec2 texCoord0 = gl_TexCoord[0].xy;
    vec2 texCoord1 = gl_TexCoord[1].xy;
    vec4 color;

    color = vec4(0.5, 0.5, 0.5, 0.8);

    if (reflectionON)
    {
        // float degs = (mod(90 * time * 0.25, 360.0) - 180) / 100.0;
        float degs = radians(mod(90 * time * worldRotationRate, 360.0));
        mat3 skyboxR = rotationMatrix33(vec3(0, 1, 0), degs);

        vec3 I = normalize(Position);
        vec3 R = skyboxR * reflect(I, normalize(Normal));
        gl_FragColor = vec4(texture(skybox, R).rgb, 1.0);
    }
    else
    {
        if (textureNum == 0)
        {
            gl_FragColor = color * texture2D(texture0, texCoord0.st);
        }
        else
        {
            gl_FragColor = color * texture2D(texture1, texCoord1.st);
        }
    }
}
