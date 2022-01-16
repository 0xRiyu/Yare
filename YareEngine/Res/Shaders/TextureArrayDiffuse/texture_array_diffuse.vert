// SHADER: VERTEX
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UboView {
    mat4 view;
    mat4 proj;
} uboView;

layout(binding = 1) uniform UboInstance {
    mat4 model;
} uboInstance;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out float fragIntensity;
layout(location = 1) out vec2 fragTexCoord;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, 5.0, -1.0));

void main() {
    gl_Position = uboView.proj * uboView.view * uboInstance.model * vec4(inPosition, 1.0);

    vec3 normalWorldSpace = normalize(mat3(uboInstance.model) * normal);

    float lightIntensity = max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), 0);

    fragIntensity = lightIntensity;
    fragTexCoord = inTexCoord;
}
