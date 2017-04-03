#version 430 core

layout(binding = 0) uniform sampler2D screenTexture;

layout(location = 0) in vec2 texcoord;

out vec4 outColor;

void main()
{
	vec4 diffAndSpec = texelFetch(screenTexture, ivec2(gl_FragCoord), 0);
	outColor = vec4(diffAndSpec.rgb, 1);
}
