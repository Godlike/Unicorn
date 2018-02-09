#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 1, binding = 0) uniform sampler2D inTextureSampler;

layout(location = 0) in vec2 inTextureCoordinate;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec4 inSpriteCoord;

layout(location = 0) out vec4 outColor;

void main() {
    if (inColor.w > 0)
    {
        outColor = vec4(inColor.xyz, 1.0);
    }
    else
    {
        vec2 spriteUV = inSpriteCoord.xy + (inTextureCoordinate * inSpriteCoord.zw);
        outColor = texture(inTextureSampler, spriteUV);
    }
}
