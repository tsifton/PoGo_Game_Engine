#version 430 core

layout(location = 0) in vec3 aPosition_modelspace;
layout(location = 1) in vec3 aNormal_modelspace;

uniform mat4 uShadowMat;

uniform mat4 uModelMat;
uniform mat4 uViewMat;
uniform mat4 uMVP;

out ShadowBlock
{
	vec4 eyePosition;
	vec4 eyeNormal;
	vec4 shadowCoord;
} outData;


void GetCameraSpace(out vec4 aPosition_cameraspace, out vec4 aNormal_cameraspace)
{
	aPosition_cameraspace = uViewMat * uModelMat * vec4(aPosition_modelspace, 1);
	aNormal_cameraspace = vec4(normalize(mat3(transpose(inverse(uViewMat * uModelMat))) * aNormal_modelspace), 0);
}

void main()
{
	// Convert Normal and Position to cameraspace
	GetCameraSpace(outData.eyePosition, outData.eyeNormal);
	outData.shadowCoord = uShadowMat * uModelMat * vec4(aPosition_modelspace, 1);

	gl_Position = uMVP * vec4(aPosition_modelspace, 1.0);
}