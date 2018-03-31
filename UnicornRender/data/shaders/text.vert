#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
layout (location = 1)uniform mat4 projection;

out vec2 TexCoords;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
