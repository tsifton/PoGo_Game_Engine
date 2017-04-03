#version 430 core

in vec3 fragPosition_cameraspace;
in vec3 fragNormal;

uniform mat4 uViewMat;

uniform vec3 uMaterialColor;

uniform vec3 uEmissiveColor;
uniform vec3 uLightPositions[10];
uniform int  uNumLights;

uniform vec3 uAmbientLightColor;

uniform vec3 uDiffuseLightColor;
uniform vec3 uAttFactor;

uniform vec3 uSpecularLightColor;
uniform float uSpecularShininess;

out vec4 color;

void main()
{
	// Loop through each light, adding the colors from each
	for (int i = 0; i < uNumLights; i++)
	{
		// Vector that goes from the fragment to the light, in camera space.
		vec3 LightPosition_cameraspace = vec3(uViewMat * vec4(uLightPositions[i], 1));
		vec3 SurfaceToLight_cameraspace = LightPosition_cameraspace - fragPosition_cameraspace;

		// Calculate the Material's Ambient Color
		vec3 MaterialAmbientColor = uAmbientLightColor * uMaterialColor;

		// Normalize the Normal and Light Vectors
		vec3 n = normalize(fragNormal);
		vec3 l = normalize(SurfaceToLight_cameraspace);

		// Find the angle between the Normal and Light Vectors
		float cosTheta = clamp(dot(n, l), 0, 1);

		// Find the distance of the Light from the Fragment
		float distance = length(SurfaceToLight_cameraspace);
		float attenuation = 1 / (uAttFactor.x + uAttFactor.y * distance + uAttFactor.z * pow(distance, 2));

		// Calculate the Material's Diffuse Color
		vec3 MaterialDiffuseColor = uMaterialColor * uDiffuseLightColor * cosTheta * attenuation;

		// Vector that goes from the fragment to the camera, in camera space.
		vec3 SurfaceToEye_cameraspace = vec3(0, 0, 0) - fragPosition_cameraspace;

		// Normalize the Eye Vector
		vec3 E = normalize(SurfaceToEye_cameraspace);

		// Vector that is the reflection of the light vector over the normal, in camera space
		vec3 R = reflect(-l, n);

		// Calculate the Specularity relative to the camera position and light reflection
		float spec = pow(max(dot(E, R), 0.0), uSpecularShininess);

		// Calculate the Material's Specular color
		vec3 MaterialSpecularColor = uSpecularLightColor * spec * uMaterialColor * attenuation;

		// Combine the Material's Ambient, Diffuse, and Specular Color
		color += clamp(vec4(uEmissiveColor + MaterialAmbientColor + MaterialDiffuseColor + MaterialSpecularColor, 1), 0, 1);
	}

}