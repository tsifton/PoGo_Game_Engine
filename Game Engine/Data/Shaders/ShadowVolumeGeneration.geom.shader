#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

layout(location = 0) in vec4 vEyePosition[];
layout(location = 1) in vec4 vEyeNormal[];

//struct Light
//{
//	vec4 position;
//};
//#define MAX_LIGHTS 5
//uniform int numLights;
//uniform Light lights[MAX_LIGHTS];

uniform mat4 uProjectionMat; // this is a trick (infinite far plane), see Notes!
uniform mat4 uViewMat;

bool FacesLight(vec4 light, vec4 a, vec4 b, vec4 c) // verts in eye space
{
	vec4 n = vec4(cross(vec3(b - a), vec3(c - a)), 0);
	vec4 da = light - a;
	vec4 db = light - b;
	vec4 dc = light - c;
	return (dot(n, da) > 0) || (dot(n, db) > 0) || (dot(n, dc) > 0);
}

void EmitEdgeQuad(vec4 light, vec4 a, vec4 b) // ab defines the edge, verts in eye space
{
	gl_Position = uProjectionMat * a;
	EmitVertex();

	gl_Position = uProjectionMat * (a - light); // w == 0 !!
	EmitVertex();

	gl_Position = uProjectionMat * b;
	EmitVertex();

	gl_Position = uProjectionMat * (b - light); // w == 0 !!
	EmitVertex();

	EndPrimitive();
}

void main()
{
	/*for (int i = 0; i < numLights; i++)
	{
		if (FacesLight(lights[i], vEyePosition[0], vEyePosition[2], vEyePosition[4]))
		{
			if (!FacesLight(lights[i], vEyePosition[0], vEyePosition[1], vEyePosition[2])) EmitEdgeQuad(lights[i], vEyePosition[0], vEyePosition[2]);
			if (!FacesLight(lights[i], vEyePosition[2], vEyePosition[3], vEyePosition[4])) EmitEdgeQuad(lights[i], vEyePosition[2], vEyePosition[4]);
			if (!FacesLight(lights[i], vEyePosition[4], vEyePosition[5], vEyePosition[0])) EmitEdgeQuad(lights[i], vEyePosition[4], vEyePosition[0]);
		}
	}*/
	vec4 lightPos = uViewMat * vec4(-45.0, 20.0, -40.0, 1.0f);
	if (FacesLight(lightPos, vEyePosition[0], vEyePosition[1], vEyePosition[2]))
	{
		EmitEdgeQuad(lightPos, vEyePosition[0], vEyePosition[1]);
		EmitEdgeQuad(lightPos, vEyePosition[1], vEyePosition[2]);
		EmitEdgeQuad(lightPos, vEyePosition[2], vEyePosition[0]);
	}
}
