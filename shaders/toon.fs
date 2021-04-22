#version 400

in vec3 Position;
in vec3 Normal;

struct LightInfo {
    vec4 position;
    vec3 color;
};

uniform LightInfo Light;
uniform vec3 Kd; // Diffuse reflectivity
uniform vec3 Ka; // Ambient reflectivity

const int levels = 3;
const float scaleFactor = 1.0 / levels;
layout( location = 0 ) out vec4 FragColor;

vec3 toonShade( )
{
    vec3 s = normalize( Light.position.xyz - Position.xyz );
    float cosine = max( 0.0, dot( s, Normal ) );
    vec3 diffuse = Kd * floor( cosine * levels ) * scaleFactor;
    return Light.color * (Ka + diffuse);
}

void main() {
    FragColor = vec4(toonShade(), 1.0);
}