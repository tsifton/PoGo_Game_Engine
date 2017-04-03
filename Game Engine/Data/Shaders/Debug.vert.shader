#version 430 core
layout(location = 0) in vec3 vPosition;

layout(location = 0) uniform mat4 uMVP;
layout(location = 1) uniform vec3 uEmissiveColor;

out vec4 color;

void main()
{
	gl_Position = uMVP * vec4(vPosition, 1.0);
	color = vec4(uEmissiveColor, 1.0);
}