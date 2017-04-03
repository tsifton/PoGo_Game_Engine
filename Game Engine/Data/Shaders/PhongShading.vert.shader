#version 430 core

layout(location = 0) in vec3 vPosition_modelspace;
layout(location = 1) in vec3 vNormal_modelspace;

uniform mat4 uModelMat;
uniform mat4 uViewMat;
uniform mat4 uMVP;

layout(location = 2) out vec4 vPosition_cameraspace;
layout(location = 3) out vec4 vNormal_cameraspace;

void GetCameraSpace(out vec4 vPosition_cameraspace, out vec4 vNormal_cameraspace)
{
	vPosition_cameraspace = uViewMat * uModelMat * vec4(vPosition_modelspace, 1);
	vNormal_cameraspace = vec4(normalize(mat3(transpose(inverse(uViewMat * uModelMat))) * vNormal_modelspace), 0);
}

void main()
{
	// Convert Normal and Position to cameraspace
	GetCameraSpace(vPosition_cameraspace, vNormal_cameraspace);

	gl_Position = uMVP * vec4(vPosition_modelspace, 1.0);
}