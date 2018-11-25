//  Set the fragment color

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;
varying vec3 vertexNorm;
varying float height;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D trailMap;

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

    gl_FragColor = vec4(N, 1);
}
