#version 430 core

layout(location = 0) in vec3 vPosition_modelspace;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vNormal_modelspace;

uniform mat4 uModelMat;
uniform mat4 uViewMat;
uniform mat4 uMVP;

layout(location = 0) out vec4 vPosition_cameraspace;
layout(location = 1) out vec2 UV;
layout(location = 2) out vec4 vNormalizedNormal_cameraspace;


void GetEyeSpace(out vec4 vPosition_cameraspace, out vec4 normalNormEye)
{
	vPosition_cameraspace = uViewMat * uModelMat * vec4(vPosition_modelspace, 1);
	normalNormEye = vec4(normalize(mat3(transpose(inverse(uViewMat * uModelMat))) * vNormal_modelspace), 0);
}

void main()
{
	// Convert Normal and Position to cameraspace
	GetEyeSpace(vPosition_cameraspace, vNormalizedNormal_cameraspace);

	gl_Position = uMVP * vec4(vPosition_modelspace, 1.0);
	UV = vUV;
}