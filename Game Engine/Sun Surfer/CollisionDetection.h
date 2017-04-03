#ifndef COLLISIONDETECTION_H_
#define COLLISIONDETECTION_H_

#pragma warning(push)
#pragma warning(disable :4201)
#include "vec3.hpp"
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

#include "SceneLoader.h"

struct RaycastHit
{
	float distance = INFINITY;		// Distance from origin to hit point
	glm::vec3 normal;				// Normalized normal of hit triangle
	glm::vec3 point;				// Impact point in world space where the ray hit
};

class Entity;
class CollidableObject;
class CollisionDetection
{
public:
	CollisionDetection() {};
	~CollisionDetection() {};

public:
	static bool RayCast(const glm::vec3& origin, const glm::vec3& direction, RaycastHit& hit, float maxDistance = INFINITY);
	static bool RayCast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance = INFINITY);
	//static bool FindWall(Entity* entity, RaycastHit& hit);
	//static bool FindFloor(Entity* entity, RaycastHit& hit);
	//static bool FindCeiling(Entity* entity, RaycastHit& hit);

	static void AddCollidable(CollidableObject* collidable);

private:
	static bool RayTriangleIntersect(	const glm::vec3& rayOrg, const glm::vec3& rayDir,
										const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
										float maxDist, RaycastHit& hit );

private:
	static CollidableObject* m_headNode;
};
#endif // !COLLISIONDETECTION_H_
