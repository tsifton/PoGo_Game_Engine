#version 430 core

// Interpolated values from the vertex shaders
in vec4 color;
in vec2 UV;

// Ouput data
out vec4 outColor;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSampler;

void main()
{
	// Output color = color of the texture at the specified UV
	outColor = vec4(texture(textureSampler, UV).rgb, 1.0);
	//outColor = vec4((texture(textureSampler, UV).rgb + color.rgb) / 2.0, 1.0f);
}