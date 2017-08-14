#version 450
#extension GL_ARB_separate_shader_objects : enable

//layout(location = 0) in vec3 fragColor;
layout(binding = 2) uniform sampler2D texSampler;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 resultColor;

void main() {
    //resultColor = vec4(fragColor, 1.0);
    resultColor = texture(texSampler, fragTexCoord);
}