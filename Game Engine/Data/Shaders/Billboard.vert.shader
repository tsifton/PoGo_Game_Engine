#version 430 core

layout(location = 0) in vec3 vPosition_modelspace;

uniform mat4 uModelMat;
uniform mat4 uViewMat;

void main()
{
	// move to eye space
	gl_Position = uViewMat * uModelMat * vec4(vPosition_modelspace, 1.0);
}
