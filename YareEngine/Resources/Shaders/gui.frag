#version 450

layout (binding = 2) uniform sampler2D fontSampler;

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec2 inUV;

layout (location = 0) out vec4 outColor;

void main()
{
    outColor = inColor * texture(fontSampler, inUV);
}