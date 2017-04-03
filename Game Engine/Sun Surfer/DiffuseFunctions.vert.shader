#version 430 core

layout(location = 0) in vec3 vPosition_modelspace;
layout(location = 1) in vec3 vNormal_modelspace;

uniform mat4 uModelMat;
uniform mat4 uViewMat;
uniform mat4 uMVP;

uniform vec3 uMaterialColor;

uniform vec3 uEmissiveColor;
uniform vec3 uLightPosition;

uniform vec3 uDiffuseLightColor;

out vec4 color;

void GetEyeSpace(out vec4 positionEye, out vec4 normalNormEye)
{
	positionEye = uViewMat * uModelMat * vec4(vPosition_modelspace, 1);
	normalNormEye = vec4(normalize(mat3(transpose(inverse(uViewMat * uModelMat))) * vNormal_modelspace), 0);
}

vec3 CalculateDiffuseLight(vec4 positonEye, vec4 normalNormEye)
{
	// Vector that goes from the fragment to the light, in camera space.
	vec4 LightPosition_cameraspace = uViewMat * vec4(uLightPosition, 1);
	vec4 SurfaceToLight_cameraspace = LightPosition_cameraspace - vPosition_cameraspace;

	// Normalize the Light Vector
	vec4 light = normalize(SurfaceToLight_cameraspace);

	// Find the angle between the Normal and Light Vectors
	float cosTheta = clamp(dot(normalNormEye, light), 0, 1);

	// Calculate the Material's Diffuse Color
	vec3 MaterialDiffuseColor = uMaterialColor * uDiffuseLightColor * cosTheta;
}

void main()
{
	vec4 vPosition_cameraspace;
	vec4 vNormalizedNormal_cameraspace;

	// Convert Normal and Position to cameraspace
	GetEyeSpace(vPosition_cameraspace, vNormalizedNormal_cameraspace);

	// Combine the Material's Ambient and Diffuse Color
	color = clamp(vec4(uEmissiveColor + CalculateDiffuseLight(vPosition_cameraspace, vNormalizedNormal_cameraspace), 1), 0, 1);

	gl_Position = uMVP * vec4(vPosition_modelspace, 1.0);
}