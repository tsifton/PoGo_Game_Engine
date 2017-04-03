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

void main()
{
	// Convert Normal and Position to cameraspace
	vec3 vPosition_cameraspace = vec3(uViewMat * uModelMat * vec4(vPosition_modelspace, 1));
	vec3 vNormal_cameraspace = mat3(transpose(inverse(uViewMat * uModelMat))) * vNormal_modelspace;
	
	// Vector that goes from the fragment to the light, in camera space.
	vec3 LightPosition_cameraspace = vec3(uViewMat * vec4(uLightPosition, 1));
	vec3 SurfaceToLight_cameraspace = LightPosition_cameraspace - vPosition_cameraspace;

	// Normalize the Normal and Light Vectors
	vec3 n = normalize(vNormal_cameraspace);
	vec3 l = normalize(SurfaceToLight_cameraspace);

	// Find the angle between the Normal and Light Vectors
	float cosTheta = clamp(dot(n, l), 0, 1);

	// Calculate the Material's Diffuse Color
	vec3 MaterialDiffuseColor = uMaterialColor * uDiffuseLightColor * cosTheta;

	// Combine the Material's Ambient and Diffuse Color
	color = clamp(vec4(uEmissiveColor + MaterialDiffuseColor, 1), 0, 1);

	gl_Position = uMVP * vec4(vPosition_modelspace, 1.0);
}