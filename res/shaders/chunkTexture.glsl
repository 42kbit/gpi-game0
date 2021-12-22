#version 450 core

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Textures[32];

in vec2 v_TexCoords;
in flat uint v_TexIndex;

void main()
{
    uint texIndexU = uint(v_TexIndex / 256.f);
    float x = float(v_TexIndex%16);
    float y = 15.f - float(floor(v_TexIndex/16));
    vec2 atlasCoords = (v_TexCoords + vec2(x,y)) / 16.f;
    o_Color = texture(u_Textures[texIndexU], atlasCoords);
}