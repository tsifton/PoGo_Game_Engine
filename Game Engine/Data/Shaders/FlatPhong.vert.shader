#version 430 core

layout(location = 0) in vec3 vPosition_modelspace;
layout(location = 1) in vec3 vNormal_modelspace;

uniform mat4 uModelMat;
uniform mat4 uViewMat;
uniform mat4 uMVP;

uniform vec3 uMaterialColor;

uniform vec3 uEmissiveColor;
uniform vec3 uLightPosition;

uniform vec3 uAmbientLightColor;

uniform vec3 uDiffuseLightColor;
uniform vec3 uAttFactor;

uniform vec3 uSpecularLightColor;
uniform float uSpecularShininess;

flat out vec4 color;


void GetEyeSpace(out vec4 vPosition_cameraspace, out vec4 normalNormEye)
{
	vPosition_cameraspace = uViewMat * uModelMat * vec4(vPosition_modelspace, 1);
	normalNormEye = vec4(normalize(mat3(transpose(inverse(uViewMat * uModelMat))) * vNormal_modelspace), 0);
}

vec3 CalculatePhongReflection(vec4 vPosition_cameraspace, vec4 normalNormEye)
{
	// Calculate the Material's Ambient Color
	vec3 ambientColor = uAmbientLightColor * uMaterialColor;

	// Vector that goes from the fragment to the light, in camera space.
	vec4 LightPosition_cameraspace = uViewMat * vec4(uLightPosition, 1);
	vec4 SurfaceToLight_cameraspace = LightPosition_cameraspace - vPosition_cameraspace;

	// Normalize the Light Vector
	vec4 light = normalize(SurfaceToLight_cameraspace);

	// Find the angle between the Normal and Light Vectors
	float cosTheta = clamp(dot(normalNormEye, light), 0, 1);

	// Calculate the Material's Diffuse Color
	vec3 diffuseColor = uMaterialColor * uDiffuseLightColor * cosTheta;

	// Vector that goes from the fragment to the camera, in camera space.
	vec3 SurfaceToEye_cameraspace = vec3(0, 0, 0) - vec3(vPosition_cameraspace);

	// Normalize the Eye Vector
	vec3 E = normalize(SurfaceToEye_cameraspace);

	// Vector that is the reflection of the light vector over the normal, in camera space
	vec3 R = vec3(reflect(-light, normalNormEye));

	// Calculate the Specularity relative to the camera position and light reflection
	float spec = pow(max(dot(E, R), 0.0), uSpecularShininess);

	// Calculate the Material's Specular color
	vec3 specularColor = uSpecularLightColor * spec * uMaterialColor;

	return ambientColor + diffuseColor + specularColor;
}

void main()
{
	vec4 vPosition_cameraspace;
	vec4 vNormalizedNormal_cameraspace;

	// Convert Normal and Position to cameraspace
	GetEyeSpace(vPosition_cameraspace, vNormalizedNormal_cameraspace);

	// Combine the Material's Ambient and Diffuse Color
	color = clamp(vec4(uEmissiveColor + CalculatePhongReflection(vPosition_cameraspace, vNormalizedNormal_cameraspace), 1), 0, 1);

	gl_Position = uMVP * vec4(vPosition_modelspace, 1.0);
}