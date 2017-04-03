#version 430 core

layout(location = 2) in vec4 fragPosition_cameraspace;
layout(location = 3) in vec4 fragNormal_cameraspace;

uniform mat4 uViewMat;

uniform vec3 uMaterialColor;

uniform vec3 uEmissiveColor;
uniform vec3 uLightPosition;

uniform vec3 uAmbientLightColor;

uniform vec3 uDiffuseLightColor;
uniform vec3 uAttFactor;

uniform vec3 uSpecularLightColor;
uniform float uSpecularShininess;

out vec4 outColor;

vec3 CalculatePhongReflection(vec4 fragPosition_cameraspace, vec4 fragNormal_cameraspace)
{
	// Calculate the Material's Ambient Color
	vec3 ambientColor = uAmbientLightColor * uMaterialColor;

	// Vector that goes from the fragment to the light, in camera space.
	vec4 LightPosition_cameraspace = uViewMat * vec4(uLightPosition, 1);
	vec4 SurfaceToLight_cameraspace = LightPosition_cameraspace - fragPosition_cameraspace;

	// Normalize the Light Vector and Normal
	vec4 nSurfaceToLight_cameraspace = normalize(SurfaceToLight_cameraspace);
	vec4 nFragNormal_cameraspace = normalize(fragNormal_cameraspace);

	// Find the angle between the Normal and Light Vectors
	float cosTheta = clamp(dot(nFragNormal_cameraspace, nSurfaceToLight_cameraspace), 0, 1);

	// Calculate the Material's Diffuse Color
	vec3 diffuseColor = uMaterialColor * uDiffuseLightColor * cosTheta;

	// Vector that goes from the fragment to the camera, in camera space.
	vec3 SurfaceToEye_cameraspace = vec3(0, 0, 0) - vec3(fragPosition_cameraspace);

	// Normalize the Eye Vector
	vec3 E = normalize(SurfaceToEye_cameraspace);

	// Vector that is the reflection of the light vector over the normal, in camera space
	vec3 R = vec3(reflect(-nSurfaceToLight_cameraspace, nFragNormal_cameraspace));

	// Calculate the Specularity relative to the camera position and light reflection
	float spec = pow(max(dot(E, R), 0.0), uSpecularShininess);

	// Calculate the Material's Specular color
	vec3 specularColor = uSpecularLightColor * spec * uMaterialColor;

	// Combine the Material's Ambient, Diffuse and Specular Color
	return ambientColor + diffuseColor + specularColor;
}

void main()
{
	outColor = clamp(vec4(uEmissiveColor + CalculatePhongReflection(fragPosition_cameraspace, fragNormal_cameraspace), 1), 0, 1);
}