#ifndef COLLIDABLEOBJECT_H_
#define COLLIDABLEOBJECT_H_

#include "Mesh.h"

#pragma warning(push)
#pragma warning(disable :4201)
#include "vec3.hpp"
#include "mat4x4.hpp"
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

class CollidableObject
{
public:
	void BuildModelToWorldMatrix(glm::vec3 position, glm::mat4 rotateMat, glm::vec3 scale);

	void Enable();
	void Disable();

	void SetMesh(Mesh* mesh);
	void SetNextNode(CollidableObject* node);

	glm::mat4 GetModelToWorldMatrix() const;
	Mesh* GetMesh();
	CollidableObject* GetNextNode() const;

	Mesh* m_mesh;

private:
	bool m_enabled{ true };
	glm::mat4 m_modelToWorldMatrix;

	CollidableObject* m_nextNode{ nullptr };
};
#endif // !COLLIDABLEOBJECT_H_
