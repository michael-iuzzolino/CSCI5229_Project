//  Set the fragment color

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;
varying vec3 vertexNorm;
varying float distToCamera;
varying float height;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D trailMap;

uniform float time;
uniform float maxPixelDistance;

void main()
{

    //  N is the object normal
    vec3 N = normalize(vertexNorm);
    vec3 up = vec3(0, 1, 0);

    float flatness = dot(N, up);

    //  L is the light vector
    vec3 L = normalize(Light);
    //  R is the reflected light vector R = 2(L.N)N - L
    vec3 R = reflect(-L, N);
    //  V is the view vector (eye vector)
    vec3 V = normalize(View);

    //  Diffuse light is cosine of light and normal vectors
    float Id = max(dot(L, N) , 0.0);

    //  Specular is cosine of reflected and view vectors
    float Is = (Id > 0.0) ? pow(max(dot(R, V), 0.0), gl_FrontMaterial.shininess) : 0.0;

    //  Sum color types
    // Color = M_e + M_a * M_c + (N*L) * M_d * C_d + (N*H)^S * M_s * C_s
    vec4 matsColor = gl_FrontMaterial.emission
                   + gl_FrontLightProduct[0].ambient
                   + Id * gl_FrontLightProduct[0].diffuse
                   + Is * gl_FrontLightProduct[0].specular;

    vec4 pixelHeight = texture2D(trailMap, gl_TexCoord[5].st);
    vec4 pixelTextureColor;
    float trailThreshold = 0.2;
    if (pixelHeight.r < trailThreshold && pixelHeight.g < trailThreshold && pixelHeight.b  < trailThreshold)
    {
        pixelTextureColor = texture2D(texture4, gl_TexCoord[4].st);
    }
    else if (height >= 18)
    {
        pixelTextureColor = texture2D(texture3, gl_TexCoord[3].st);
    }
    else if (height < 18 && height >= 15)
    {
        if (flatness <= 0.5)
        {
            pixelTextureColor = texture2D(texture2, gl_TexCoord[2].st);
        }
        else
        {
            pixelTextureColor = texture2D(texture3, gl_TexCoord[3].st);
        }
    }
    else if (height < 15 && height >= 6)
    {
        if (flatness >= 0.9)
        {
            pixelTextureColor = texture2D(texture1, gl_TexCoord[1].st);
        }
        else
        {
            pixelTextureColor = texture2D(texture2, gl_TexCoord[2].st);
        }
    }
    else if (height < 7 && height >= 3)
    {
        if (flatness >= 0.7)
        {
            pixelTextureColor = texture2D(texture1, gl_TexCoord[1].st);
        }
        else
        {
            pixelTextureColor = texture2D(texture1, gl_TexCoord[1].st);
        }
    }
    else
    {
        pixelTextureColor = texture2D(texture0, gl_TexCoord[0].st);
    }

    if (abs(distToCamera) > maxPixelDistance)
    {
        discard;
    }

    gl_FragColor = matsColor * pixelTextureColor;

    if (pixelTextureColor == texture2D(texture4, gl_TexCoord[4].st))
    {
        return;
    }

    // Animation for seasons
    float totalTime = 10.0;
    float currentTime = mod(time, totalTime);
    float w1, w2;
    vec4 snowColor = texture2D(texture3, gl_TexCoord[3].st);
    if (currentTime < totalTime/4.0)
    {

    }
    else if (currentTime < totalTime*2/4.0)
    {

    }
    else if (currentTime < totalTime*3/4.0)
    {
        w1 = currentTime/totalTime*2;
        w2 = 1-currentTime/totalTime*2;

        float time0 = currentTime - totalTime*2/4.0;
        float currentPixel = length(gl_TexCoord[0].st - vec2(0.5, 0.5));
        if (currentPixel <= time0)
        {
            gl_FragColor = matsColor * (w1 * snowColor + w2 * pixelTextureColor);
        }
    }
    else
    {
        w1 = 1-currentTime/totalTime;
        w2 = currentTime/totalTime;
        float time0 = currentTime - totalTime*3/4.0;
        float currentPixel = length(gl_TexCoord[0].st - vec2(0.5, 0.5));
        if (currentPixel >= time0)
        {
            gl_FragColor = matsColor * (w1 * snowColor * 2 + w2 * pixelTextureColor);
        }
    }
}
