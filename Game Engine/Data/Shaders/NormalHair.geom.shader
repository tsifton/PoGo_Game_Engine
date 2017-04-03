#version 430 core

layout(triangles) in;
layout(line_strip, max_vertices = 12) out;

struct Vertex
{
	vec4 normal;
	vec4 color;
};
in Vertex vert[];

uniform mat4 uProjectionMat;

out vec4 lineColor;

void main()
{
	vec3 p0 = gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz;

	vec4 meshColor = vec4(.3, .3, 0, 1);

	gl_Position = uProjectionMat * vec4(p0, 1);
	lineColor = meshColor;
	EmitVertex();

	gl_Position = uProjectionMat * vec4(p1, 1);
	lineColor = meshColor;
	EmitVertex();

	gl_Position = uProjectionMat * vec4(p2, 1);
	lineColor = meshColor;
	EmitVertex();

	gl_Position = uProjectionMat * vec4(p0, 1);
	lineColor = meshColor;
	EmitVertex();

	EndPrimitive(); // ends the line strip for the mesh triangle

	for (int i = 0; i < gl_in.length(); i++)
	{
		vec4 P = gl_in[i].gl_Position;
		vec4 N = normalize(vert[i].normal);

		gl_Position = uProjectionMat * P;
		lineColor = vert[i].color;
		EmitVertex();

		gl_Position = uProjectionMat * (P + (N * 2));
		lineColor = vert[i].color;
		EmitVertex();

		EndPrimitive();
	}

	vec3 V0 = p0 - p1;
	vec3 V1 = p2 - p1;

	vec3 N = normalize(cross(V1, V0));

	// Center of the triangle
	vec3 P = (p0 + p1 + p2) / 3.0;

	gl_Position = uProjectionMat * vec4(P, 1.0);
	lineColor = vec4(1, 1, 0, 1);
	EmitVertex();

	gl_Position = uProjectionMat * vec4(P + (N * 1), 1.0);
	lineColor = vec4(1, 1, 0, 1);
	EmitVertex();

	EndPrimitive();
}
