#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 1, binding = 0) uniform sampler2D inTextureSampler;

layout(location = 0) in vec2 inTextureCoordinate;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 texColor = texture(inTextureSampler, inTextureCoordinate);
    outColor = vec4(texColor.r, texColor.r, texColor.r, texColor.r);
}
