//  Set the fragment color

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

uniform float time;

void main()
{
    //  N is the object normal
    vec3 N = normalize(Normal);
    //  L is the light vector
    vec3 L = normalize(Light);
    //  R is the reflected light vector R = 2(L.N)N - L
    vec3 R = reflect(-L,N);
    //  V is the view vector (eye vector)
    vec3 V = normalize(View);

    //  Diffuse light is cosine of light and normal vectors
    float Id = max(dot(L,N) , 0.0);
    //  Specular is cosine of reflected and view vectors
    float Is = (Id>0.0) ? pow(max(dot(R,V),0.0) , gl_FrontMaterial.shininess) : 0.0;

    //  Sum color types
    vec4 matsColor = gl_FrontMaterial.emission
               + gl_FrontLightProduct[0].ambient
               + Id*gl_FrontLightProduct[0].diffuse
               + Is*gl_FrontLightProduct[0].specular;

    vec4 tex1Color = texture2D(texture1, gl_TexCoord[0].st);
    vec4 tex2Color = texture2D(texture2, gl_TexCoord[0].st);
    vec4 tex3Color = texture2D(texture3, gl_TexCoord[0].st);



    float colorThreshold = 0.9;
    if (tex1Color.r > colorThreshold && tex1Color.g > colorThreshold && tex1Color.b > colorThreshold)
    {
        discard;
    }

    float totalTime = 10.0;
    float currentTime = mod(time, totalTime);
    float w1, w2, w3;
    if (currentTime < totalTime/4.0)
    {
        w1 = 1-currentTime/totalTime*3;
        w2 = currentTime/totalTime*2.5;
        w3 = 0;
    }
    else if (currentTime < totalTime*2/4.0)
    {
        w1 = 0;
        w2 = 1-currentTime/totalTime;
        w3 = currentTime/totalTime;
    }
    else if (currentTime < totalTime*3/4.0)
    {
        w1 = 0;
        w2 = 1-currentTime/totalTime;
        w3 = currentTime/totalTime;

        float time0 = currentTime - totalTime*2/4.0;
        float currentPixel = 1 - length(gl_TexCoord[0].st - vec2(0.5, 0.5));
        if (currentPixel <= time0)
        {
            discard;
        }
    }
    else
    {
        w1 = 1;
        w3 = 0;
        w2 = 0;

        float time0 = currentTime - totalTime*3/4.0;
        float currentPixel = length(gl_TexCoord[0].st - vec2(0.5, 0.5));
        if (currentPixel >= time0)
        {
            discard;
        }
    }

    vec4 texColor = w1 * tex1Color + w2 * tex3Color + w3 * tex2Color;
    gl_FragColor = matsColor * texColor;
}
