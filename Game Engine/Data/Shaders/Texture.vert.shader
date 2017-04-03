#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;

layout(location = 0) uniform mat4 uMVP;

// Output data, will be interpolated for each fragment.
out vec4 color;
out vec2 UV;

void main()
{
	gl_Position = uMVP * vec4(vertexPosition, 1.0);
	color = vec4(vertexColor, 1.0);
	UV = vertexUV;
}