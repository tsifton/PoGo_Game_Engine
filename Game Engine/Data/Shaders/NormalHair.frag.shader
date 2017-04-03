#version 430 core

in vec4 lineColor; // from geometry shader

layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = lineColor;
}
