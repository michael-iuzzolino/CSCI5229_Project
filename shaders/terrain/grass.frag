//  Set the fragment color

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;

uniform sampler2D texture;

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
               + gl_FrontLightProduct[0].ambient + 0.1
               + Id*gl_FrontLightProduct[0].diffuse * 0.5
               + Is*gl_FrontLightProduct[0].specular;

    vec4 texColor = texture2D(texture, gl_TexCoord[0].st);
    gl_FragColor = matsColor * texture2D(texture, gl_TexCoord[0].st);

    float colorThreshold = 0.9;
    if (texColor.r > colorThreshold && texColor.g > colorThreshold && texColor.b > colorThreshold)
    {
        discard;
    }


    // Animation for seasons
    float totalTime = 10.0;
    float currentTime = mod(time, totalTime);
    if (currentTime < totalTime/4.0)
    {

    }
    else if (currentTime < totalTime*3/5.0)
    {
        float time0 = currentTime - totalTime*2/4.0;
        float currentPixel = 1 - length(gl_TexCoord[0].st);
        if (currentPixel <= time0)
        {
            discard;
        }
    }
    else
    {
        float time0 = currentTime - totalTime*3/4.0;
        float currentPixel = length(gl_TexCoord[0].st) + 0.5;
        if (currentPixel >= time0)
        {
            discard;
        }
    }
}
