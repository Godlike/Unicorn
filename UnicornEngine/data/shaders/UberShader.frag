#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

//layout(location = 0) in vec3 fragColor;
layout(set = 1, binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 resultColor;

void main() {
    //resultColor = vec4(fragColor, 1.0);
    if (inColor.w > 0)
    {
        resultColor = vec4(inColor.xyz, 1.0);
    }
    else
    {
        resultColor = texture(texSampler, fragTexCoord);
    }
}