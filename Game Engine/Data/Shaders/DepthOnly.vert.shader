#version 430 core

layout(location = 0) in vec3 vPosition_modelspace;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(vPosition_modelspace, 1.0f);
}