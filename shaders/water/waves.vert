//  Identity vertex shader
const float pi = 3.14159;
uniform float time;

varying float LightIntensity;

// https://jayconrod.com/posts/34/water-simulation-in-glsl

float phong()
{
   //  P is the vertex coordinate on body
   vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex);
   //  N is the object normal at P
   vec3 N = normalize(gl_NormalMatrix * gl_Normal);
   //  Light Position for light 0
   vec3 LightPos = vec3(gl_LightSource[0].position);
   //  L is the light vector
   vec3 L = normalize(LightPos - P);
   //  R is the reflected light vector R = 2(L.N)N - L
   vec3 R = reflect(-L, N);
   //  V is the view vector (eye at the origin)
   vec3 V = normalize(-P);

   //  Diffuse light intensity is cosine of light and normal vectors
   float Id = max(dot(L,N) , 0.0);
   //  Shininess intensity is cosine of light and reflection vectors to a power
   float Is = (Id>0.0) ? pow(max(dot(R,V) , 0.0) , gl_FrontMaterial.shininess) : 0.0;

   //  Vertex color
   vec3 color = gl_FrontLightProduct[0].ambient.rgb
           + Id*gl_FrontLightProduct[0].diffuse.rgb
           + Is*gl_FrontLightProduct[0].specular.rgb;

   //  Vertex intensity
   return length(color);
}

float waveHeight(float x, float z)
{
    // Params
    float wavelength = 5;
    float speed = 0.5;
    float amplitude = 1;
    vec2 direction = vec2(1, 2);

    // Calculations
    float frequency = 2*pi/wavelength;
    float phase = speed * frequency;
    float theta = dot(direction, vec2(x, z));
    return amplitude * sin(theta * frequency + time * phase);
}

void main()
{
    //  Texture
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_MultiTexCoord1;

    vec4 pos = gl_Vertex;

    pos.y = 1.0 + waveHeight(pos.x, pos.z);

    //  Set vertex coordinates
    gl_Position = gl_ModelViewProjectionMatrix * pos;
}
