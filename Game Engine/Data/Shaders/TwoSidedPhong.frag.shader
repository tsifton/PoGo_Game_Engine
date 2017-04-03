#version 430 core

in vec4 frontColor;
in vec4 backColor;

out vec4 outColor;

void main()
{
	if (gl_FrontFacing)
		outColor = frontColor;
	else
		outColor = backColor;
}