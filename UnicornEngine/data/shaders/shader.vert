#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformViewProjection {
    mat4 view;
    mat4 proj;
} uvp_buffer;

layout(binding = 1) uniform UniformModel {
    mat4 model;
} um_buffer;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = uvp_buffer.proj * uvp_buffer.view * um_buffer.model * vec4(pos, 1.0);
    fragColor = color;
}
