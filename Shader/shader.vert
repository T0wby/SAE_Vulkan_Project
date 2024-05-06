#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 ambientLightColor;
    vec3 lightPosition;
    vec4 lightColor;
    vec3 camPos;
} ubo;

layout(push_constant) uniform Push {
    mat4 transform;
} push;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormalWorld;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 fragPosWorld;

void main() {
    vec4 positionWorld = push.transform * vec4(inPosition, 1.0);
    gl_Position = (ubo.proj * ubo.view * ubo.model) * positionWorld;
    
    mat3 normalMatrix = mat3(transpose(inverse(ubo.model)));
    fragNormalWorld = normalize(normalMatrix * inNormal);
    fragPosWorld = positionWorld.xyz;
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}