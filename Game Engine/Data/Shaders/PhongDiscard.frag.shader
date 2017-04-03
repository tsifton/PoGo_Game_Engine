#version 430 core

in vec4 color;

out vec4 outColor;

bool ShouldDiscard()
{
	return color.r > 0.5f;
}

void main()
{
	if (ShouldDiscard()) discard;
	outColor = color;
}