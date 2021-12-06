#version 450 core

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Textures[32];

in vec2 v_TexCoords;
in float v_TexIndex;

void main()
{
    uint texIndexU = uint(v_TexIndex);
    o_Color = texture(u_Textures[texIndexU], v_TexCoords);
}