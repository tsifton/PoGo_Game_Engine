#version 430 core

layout(location = 0) in vec3 vPosition_modelspace;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vNormal;

uniform mat4 uModelMat;
uniform mat4 uViewMat;
uniform mat4 uMVP;

out vec3 fragPosition_cameraspace;
out vec2 fragUV;
out vec3 fragNormal;

void main()
{
	gl_Position = uMVP * vec4(vPosition_modelspace, 1.0);

	fragPosition_cameraspace = vec3(uViewMat * uModelMat * vec4(vPosition_modelspace, 1));
	fragUV = vUV;
	fragNormal = mat3(transpose(inverse(uViewMat * uModelMat))) * vNormal;
}