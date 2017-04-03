#version 430 core

layout(location = 0) in vec3 vPosition_modelspace;
layout(location = 1) in vec3 vNormal_modelspace;

uniform mat4 uProjectionMat;
uniform mat4 uViewMat;
uniform mat4 uModelMat;

struct Vertex
{
	vec4 normal;
	vec4 color;
};
out Vertex vert;

void main()
{
	mat3 normalMatrix = mat3(transpose(inverse(uViewMat * uModelMat)));
	vert.normal = normalize(vec4(normalMatrix * vNormal_modelspace, 0));
	vert.color = vec4(vNormal_modelspace, 1);

	// move to eye space
	gl_Position = uViewMat * (uModelMat * vec4(vPosition_modelspace, 1));
}
