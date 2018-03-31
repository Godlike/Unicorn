#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

in vec2 TexCoords;
out vec4 color;

layout (location = 0) uniform sampler2D glyphMapSmpl;
layout (location = 1) uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(glyphMapSmpl, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
