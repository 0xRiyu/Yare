// SHADER: FRAGMENT
#version 450
#pragma optimize (off)
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 2) uniform sampler2D texSampler[2];

layout(push_constant) uniform PER_OBJECT {
    int imgIdx;
}pc;

void main() {
    float count = 1;
    while (count <= 10.0) {
        count *= sin(count);
    }
    outColor = texture(texSampler[pc.imgIdx + int(count)], fragTexCoord);
}
