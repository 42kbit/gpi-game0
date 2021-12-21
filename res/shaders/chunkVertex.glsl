#version 450 core

layout(location = 0) in uint a_Data0;
layout(location = 1) in uint a_Data1;

out float v_TexIndex;
out vec2 v_TexCoords;

layout (std140) uniform ProjectionView
{
    mat4 projection;
    mat4 view;
} u_ProjectionView;

uniform mat4 u_Model;

vec2 texCoords[4] = vec2[4]
(
    vec2(0,0),
    vec2(0,1),
    vec2(1,1),
    vec2(1,0)
);

void main()
{
    float posX = float((a_Data0 & 0xF8000000) >> 27);
    float posY = float((a_Data0 & 0x07FC0000) >> 18);
    float posZ = float((a_Data0 & 0x0003E000) >> 13);
    v_TexCoords = texCoords[(a_Data0 & 0x00001800) >> 11];
    v_TexIndex = float((a_Data0 & 0x000007FF) >> 0);
    gl_Position = u_ProjectionView.projection * u_ProjectionView.view * u_Model * vec4(posX, posY, posZ, 1.f);
    
}