#version 430 core

layout(location = 0) in vec4 fragPosition_cameraspace;
layout(location = 1) in vec4 fragNormal_cameraspace;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct Light
{
	vec4 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define MAX_LIGHTS 15
uniform int numLights;
uniform Light lights[MAX_LIGHTS];

struct Fog
{
	float density;
	vec3 color;
};

uniform Fog fog;

uniform mat4 uViewMat;

uniform vec3 uEmissiveColor;

out vec4 outColor;

vec3 CalculatePhongReflection(Light light, vec4 fragPosition_cameraspace, vec4 fragNormal_cameraspace)
{
	// Calculate the Material's Ambient Color
	vec3 ambientColor = light.ambient * material.ambient;

	// Vector that goes from the fragment to the light, in camera space.
	vec4 LightPosition_cameraspace = uViewMat * light.position;
	vec4 SurfaceToLight_cameraspace = LightPosition_cameraspace - fragPosition_cameraspace;

	// Normalize the Light Vector and Normal
	vec4 nSurfaceToLight_cameraspace = normalize(SurfaceToLight_cameraspace);
	vec4 nFragNormal_cameraspace = normalize(fragNormal_cameraspace);

	// Find the angle between the Normal and Light Vectors
	float cosTheta = clamp(dot(nFragNormal_cameraspace, nSurfaceToLight_cameraspace), 0, 1);

	// Calculate the Material's Diffuse Color
	vec3 diffuseColor = material.diffuse * light.diffuse * cosTheta;

	// Vector that goes from the fragment to the camera, in camera space.
	vec3 SurfaceToEye_cameraspace = vec3(0, 0, 0) - vec3(fragPosition_cameraspace);

	// Normalize the Eye Vector
	vec3 E = normalize(SurfaceToEye_cameraspace);

	// Vector that is the reflection of the light vector over the normal, in camera space
	vec3 R = vec3(reflect(-nSurfaceToLight_cameraspace, nFragNormal_cameraspace));

	// Calculate the Specularity relative to the camera position and light reflection
	float spec = pow(max(dot(E, R), 0.0), material.shininess);

	// Calculate the Material's Specular color
	vec3 specularColor = light.specular * spec * material.specular;

	// Combine the Material's Ambient, Diffuse and Specular Color
	return ambientColor + diffuseColor + specularColor;
}

void main()
{
	float distance = length(fragPosition_cameraspace);
	float e = 2.71828;
	float fogFactor = clamp(pow(e, -fog.density * abs(distance)), 0.0, 1.0);

	// Calulate the Phong Color per light
	vec4 accumulativeColor = vec4(0, 0, 0, 1);
	for (int i = 0; i < numLights; i++)
	{
		accumulativeColor += clamp(vec4(uEmissiveColor + CalculatePhongReflection(lights[i], fragPosition_cameraspace, fragNormal_cameraspace), 1), 0, 1);
	}

	outColor = mix(vec4(fog.color, 1), accumulativeColor, fogFactor);
}