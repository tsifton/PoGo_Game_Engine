#include "CollisionDetection.h"

#include "GameLogger.h"

#include "Entity.h"
#include "CollisionComponent.h"

CollidableObject* CollisionDetection::m_headNode = nullptr;

bool CollisionDetection::RayCast(const glm::vec3 & origin, const glm::vec3 & direction, RaycastHit & hit, float maxDistance)
{
	bool result = false;
	glm::vec3 triVerts[3];
	for (CollidableObject* collidable = m_headNode; collidable != nullptr; collidable = collidable->GetNextNode())
	{
		glm::mat4 modelToWorld = collidable->GetModelToWorldMatrix();

		int vertsPerFace = 3;
		int floatsPerVert = GetFloatsPerVert(collidable->GetMesh()->vertFormat);
		int floatsPerFace = floatsPerVert * vertsPerFace;
		int numVerts = collidable->GetMesh()->numVerts;
		int numFaces = numVerts / vertsPerFace;
		float* verts = reinterpret_cast<float*>(collidable->GetMesh()->verts);

		if (modelToWorld == glm::mat4())
		{
			for (int i = 0; i < numFaces; i++)
			{
				for (int j = 0; j < vertsPerFace; j++)
				{
					triVerts[j] = glm::vec3(verts[i * floatsPerFace + j * floatsPerVert], verts[i * floatsPerFace + j * floatsPerVert + 1], verts[i * floatsPerFace + j * floatsPerVert + 2]);
				}
				if (RayTriangleIntersect(origin, direction, triVerts[0], triVerts[1], triVerts[2], maxDistance, hit)) result = true;
			}
		}
		else
		{
			for (int i = 0; i < numFaces; i++)
			{
				for (int j = 0; j < vertsPerFace; j++)
				{
					triVerts[j] = glm::vec3(modelToWorld * glm::vec4(verts[i * floatsPerFace + j * floatsPerVert], verts[i * floatsPerFace + j * floatsPerVert + 1], verts[i * floatsPerFace + j * floatsPerVert + 2], 1.0f));
				}
				if (RayTriangleIntersect(origin, direction, triVerts[0], triVerts[1], triVerts[2], maxDistance, hit)) result = true;
			}
		}
		
	}
	return result;
}

bool CollisionDetection::RayCast(const glm::vec3 & origin, const glm::vec3 & direction, float maxDistance)
{
	RaycastHit hit;
	return RayCast(origin, direction, hit, maxDistance);
}

void CollisionDetection::AddCollidable(CollidableObject * collidable)
{
	collidable->SetNextNode(m_headNode);
	m_headNode = collidable;
}

bool CollisionDetection::RayTriangleIntersect(	const glm::vec3 & rayOrg, const glm::vec3 & rayDir, 
												const glm::vec3 & p0, const glm::vec3 & p1, const glm::vec3 & p2, 
												float maxDist, RaycastHit& hit )
{
	// Return false if no intersection is detected
	const bool kNoIntersection = false;

	// Furthest Distance to look for intersection
	const float minT = maxDist < hit.distance ? maxDist : hit.distance;

	// Compute clockwise edge vectors
	glm::vec3 e1 = p1 - p0;
	glm::vec3 e2 = p2 - p1;

	// Compute surface normal (Unnormalized)
	glm::vec3 n = glm::cross(e1, e2);

	// Compute gradient, which tells us how steep of an angle
	// we are approaching the *front* side of the triangle
	float dot = glm::dot(n, rayDir);

	// Check for a ray that is parallel to the triangle, or not
	// pointing towards the front face of the triangle
	//
	// Note that this will also reject degenerate triangles and 
	// rays as well. We code this in a very particular way so
	// that NANs will bail here.
	if (!(dot < 0.0f)) return kNoIntersection;

	// Compute d value for the plane equation. We will
	// use the plane equation with d on the right side:
	// Ax + By + Cz = d
	float d = glm::dot(n, p0);

	// Compute parametric point of intersection with the plane
	// containing the triangle, checking at the earliest
	// possible stages for trivial rejection
	float t = d - glm::dot(n, rayOrg);

	// Is ray origin on the backside of the polygon? Again,
	// we phrase the check so that NANs will bail
	if (!(t <= 0.0f)) return kNoIntersection;

	// Closer intersections already found? (Or does
	// the ray not reach the plane?)
	//
	// since dot < 0:
	//		t/dot > dist
	// is the same as
	//		t < dot * dist
	//
	// (And then we invert it for NAN checking...)
	if (!(t >= dot * minT)) return kNoIntersection;

	// OK, the ray intersects the plane. Compute actual paramteric
	// point of intersection
	t /= dot;
	assert(t >= 0.0f);
	/*assert(t <= minT);*/

	// Compute 3D point of intersection
	glm::vec3 p = rayOrg + rayDir * t;

	// Find domi nant axis to select which plane
	// to project onto, and compute u's and v's
	float u0, u1, u2;
	float v0, v1, v2;
	if (fabs(n.x) > fabs(n.y))
	{
		if (fabs(n.x) > fabs(n.z))
		{
			u0 = p.y - p0.y;
			u1 = p1.y - p0.y;
			u2 = p2.y - p0.y;

			v0 = p.z - p0.z;
			v1 = p1.z - p0.z;
			v2 = p2.z - p0.z;
		}
		else
		{
			u0 = p.x - p0.x;
			u1 = p1.x - p0.x;
			u2 = p2.x - p0.x;

			v0 = p.y - p0.y;
			v1 = p1.y - p0.y;
			v2 = p2.y - p0.y;
		}
	}
	else
	{
		if (fabs(n.y) > fabs(n.z))
		{
			u0 = p.x - p0.x;
			u1 = p1.x - p0.x;
			u2 = p2.x - p0.x;

			v0 = p.z - p0.z;
			v1 = p1.z - p0.z;
			v2 = p2.z - p0.z;
		}
		else
		{
			u0 = p.x - p0.x;
			u1 = p1.x - p0.x;
			u2 = p2.x - p0.x;

			v0 = p.y - p0.y;
			v1 = p1.y - p0.y;
			v2 = p2.y - p0.y;
		}
	}

	// Compute the denominator, check for invalid
	float temp = u1 * v2 - v1 * u2;
	if (!(temp != 0.0f)) return kNoIntersection;
	temp = 1.0f / temp;

	// Compute barycentric coords, checking for out-of-range at each step
	float alpha = (u0 * v2 - v0 * u2) * temp;
	if (!(alpha >= 0.0f)) return kNoIntersection;

	float beta = (u1 * v0 - v1 * u0) * temp;
	if (!(beta >= 0.0f)) return kNoIntersection;

	float gamma = 1.0f - alpha - beta;
	if (!(gamma >= 0.0f)) return kNoIntersection;

	// Give Hit Information
	hit.distance = t;
	hit.normal = glm::normalize(n);
	hit.point = p;

	return true;
}
