#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform UniformViewProjection {
    mat4 view;
    mat4 proj;
} uvp_buffer;

layout(binding = 1) uniform UniformModel {
    mat4 model;
} um_buffer;

layout(push_constant) uniform PushConsts {
    vec4 color;
} pushConsts;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

//layout(location = 0) out vec3 fragColor;
layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec4 outColor;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = uvp_buffer.proj * uvp_buffer.view * um_buffer.model * vec4(pos, 1.0);
    gl_Position.y = -gl_Position.y;
    fragTexCoord = texCoord;
    outColor = pushConsts.color;
}