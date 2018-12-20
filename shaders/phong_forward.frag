#version 330 core

#define EPSILON 0.000001

// Incoming attributes interpolated from vertex shader
in vec3 vertexInEyeSpace;
in vec3 normalInEyeSpace;
in vec2 texCoord;

// Material properties
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float shininess;
uniform bool textured;
uniform sampler2D diffuseTex;

// light information (single directional light)
uniform vec3 lightInViewSpace;
uniform vec3 lightColor;

// output (fragment color)
layout (location = 0) out vec4 finalColor;

void main() {
    vec3 n = normalize(normalInEyeSpace);
    vec3 e = normalize(-vertexInEyeSpace);
    
    vec3 diffuseColor = kd;
    if (textured) {
        diffuseColor *= texture(diffuseTex, texCoord).xyz;
    }

    vec3 outColor = vec3(0);

    vec3 l = normalize(-lightInViewSpace);
    vec3 h = normalize(l + e);
    outColor += lightColor * ka;
    outColor += lightColor * diffuseColor * max(0.0, dot(l, n));
    if (dot(l, n) > EPSILON)
        outColor += lightColor * ks * pow(max(dot(h, n), 0.0), 4*shininess);

    finalColor.rgb = outColor;
    finalColor.a   = 1.0;
}
