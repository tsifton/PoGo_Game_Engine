#version 430 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;

layout(location = 0) uniform mat4 uMVP;

out vec4 color;

void main()
{
	gl_Position = uMVP * vec4(vPosition, 1.0);
	color = vec4(vColor, 1.0);
}