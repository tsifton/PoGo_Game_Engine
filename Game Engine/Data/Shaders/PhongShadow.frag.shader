#version 430 core

in ShadowBlock
{
	vec4 eyePosition;
	vec4 eyeNormal;
	vec4 shadowCoord;
} inData;

layout(binding = 4) uniform sampler2DShadow tShadowMap;

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

#define MAX_LIGHTS 5
uniform int numLights;
uniform Light lights[MAX_LIGHTS];

uniform mat4 uViewMat;

uniform vec3 uEmissiveColor;

out vec4 fragColor;

vec3 GetDiffuseAndSpecularColor(Light light, vec4 fragPosition_cameraspace, vec4 fragNormal_cameraspace)
{
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
	return diffuseColor + specularColor;
}

vec3 GetAmbientColor(Light light)
{
	return light.ambient * material.ambient;
}

void main()
{
	vec3 ambient = vec3(0, 0, 0);
	vec3 diffuseAndSpecular = vec3(0, 0, 0);

	// Calulate the Phong Color per light
	for (int i = 0; i < numLights; i++)
	{
		ambient += GetAmbientColor(lights[i]);
		diffuseAndSpecular += GetDiffuseAndSpecularColor(lights[i], inData.eyePosition, inData.eyeNormal);
	}

	// shadow map lookup using perspective division
	float t1 = textureProjOffset(tShadowMap, inData.shadowCoord, ivec2(1, 1));
	float t2 = textureProjOffset(tShadowMap, inData.shadowCoord, ivec2(-1, 1));
	float t3 = textureProjOffset(tShadowMap, inData.shadowCoord, ivec2(-1, -1));
	float t4 = textureProjOffset(tShadowMap, inData.shadowCoord, ivec2(1, -1));

	float shadow = (t1 + t2 + t3 + t4) / 4;

	/*float shadow = textureProj(tShadowMap, inData.shadowCoord);*/

	if (inData.shadowCoord.z < 0.0f || (inData.shadowCoord.z / inData.shadowCoord.w) > 1.0f) { shadow = 1.0f; }

	// if fragment is in shadow, use ambient only
	fragColor = vec4(diffuseAndSpecular * shadow + ambient, 1.0);
}