#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4  uProjectionMat;
uniform float uhalfWidth;  // half the width of the quad

layout(location = 0) out vec4 vPosition_cameraspace;
layout(location = 1) out vec4 vNormalizedNormal_cameraspace; 
layout(location = 2) out vec2 texCoord;

vec4 GetNormal(vec4 p0, vec4 p1, vec4 p2)
{
	vec3 a = vec3(p0) - vec3(p1);
	vec3 b = vec3(p2) - vec3(p1);
	return vec4(normalize(cross(b, a)), 0.0);
}

void main()
{
	vec4 offset1 = vec4(uhalfWidth, uhalfWidth, 0.0, 0.0);
	vec4 offset2 = vec4(-uhalfWidth, uhalfWidth, 0.0, 0.0);
	vec4 posClip = gl_in[0].gl_Position;

	vec4 p0 = posClip + offset2;
	vec4 p1 = posClip - offset1;
	vec4 p2 = posClip + offset1;
	vec4 p3 = posClip - offset2;

	vec4 normal = GetNormal(p0, p1, p2);

	// BCAD - ordering for CCW triangle strip

	// output vertex B
	gl_Position = uProjectionMat * p0;
	vPosition_cameraspace = p0;
	vNormalizedNormal_cameraspace = normal;
	texCoord = vec2(0.0, 1.0);
	EmitVertex();

	// output vertex C
	gl_Position = uProjectionMat * p1;
	vPosition_cameraspace = p1;
	vNormalizedNormal_cameraspace = normal;
	texCoord = vec2(0.0, 0.0);
	EmitVertex();

	// output vertex A
	gl_Position = uProjectionMat * p2;
	vPosition_cameraspace = p2;
	vNormalizedNormal_cameraspace = normal;
	texCoord = vec2(1.0, 1.0);
	EmitVertex();

	// output vertex D
	gl_Position = uProjectionMat * p3;
	vPosition_cameraspace = p3;
	vNormalizedNormal_cameraspace = normal;
	texCoord = vec2(1.0, 0.0);
	EmitVertex();

	EndPrimitive(); // ends the triangle strip
}
