#version 420 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoords;
layout(location = 2) in float a_TexIndex;

out vec2 v_TexCoords;
out float v_TexIndex;

layout (std140) uniform ProjectonView
{
    mat4 projection;
    mat4 view;
} u_ProjectonView;

uniform mat4 u_Model;

void main()
{
	v_TexCoords = a_TexCoords;
	v_TexIndex = a_TexIndex;
	gl_Position = u_ProjectonView.projection * u_ProjectonView.view * u_Model * vec4(a_Position, 0, 1);
}