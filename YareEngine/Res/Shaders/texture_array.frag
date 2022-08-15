// SHADER: FRAGMENT
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 2) uniform sampler2D texSampler[16];

layout(push_constant) uniform PER_OBJECT {
    int imgIdx;
}pc;

void main() {
    outColor = texture(texSampler[pc.imgIdx], fragTexCoord);
}
