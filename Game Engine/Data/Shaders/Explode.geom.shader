#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout(location = 0) in vec4 vPosition[];
layout(location = 1) in vec2 vUV[];
layout(location = 2) in vec4 vNormal[];

uniform mat4 uViewportMat;
uniform float time;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec2 gUV;
layout(location = 2) out vec4 gNormal;
layout(location = 3) noperspective out vec3 gEdgeDistance;

vec4 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return vec4(normalize(cross(a, b)), 0.0);
}

vec4 explode(vec4 position, vec4 normal)
{
	float magnitude = 2.0f;
	vec4 direction = normal * ((sin(time / 1000.0f) + 1.0f) / 2.0f) * magnitude;
	return position + direction;
}

void main()
{
	// Move each vertex into window space
	vec3 p0 = vec3(uViewportMat * (gl_in[0].gl_Position / gl_in[0].gl_Position.w));
	vec3 p1 = vec3(uViewportMat * (gl_in[1].gl_Position / gl_in[1].gl_Position.w));
	vec3 p2 = vec3(uViewportMat * (gl_in[2].gl_Position / gl_in[2].gl_Position.w));

	// Get each edge length
	float a = length(p0 - p1);
	float b = length(p1 - p2);
	float c = length(p2 - p0);

	// Calculate angles between edges (only need two) using law of cosines
	float alpha = acos((pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c));
	float beta = acos((pow(a, 2) + pow(c, 2) - pow(b, 2)) / (2 * a * c));

	// Find the distance from each vertex to its corresponding edge
	float ha = abs(c * sin(beta));
	float hb = abs(c * sin(alpha));
	float hc = abs(b * sin(alpha));

	// Emit each vert with the distance from its edge
	vec4 normal = GetNormal();

	gl_Position = explode(gl_in[0].gl_Position, normal);
	gPosition = vPosition[0];
	gUV = vUV[0];
	gNormal = vNormal[0];
	gEdgeDistance = vec3(ha, 0, 0);
	EmitVertex();

	gl_Position = explode(gl_in[1].gl_Position, normal);
	gPosition = vPosition[1];
	gUV = vUV[1];
	gNormal = vNormal[1];
	gEdgeDistance = vec3(0, hb, 0);
	EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position, normal);
	gPosition = vPosition[2];
	gUV = vUV[2];
	gNormal = vNormal[2];
	gEdgeDistance = vec3(0, 0, hc);
	EmitVertex();

	EndPrimitive();
}
