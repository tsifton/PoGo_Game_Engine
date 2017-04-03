#ifndef GRAPHICALCOMPONENT_H_
#define GRAPHICALCOMPONENT_H_

#include "Component.h"
#include "GraphicsObject.h"

class GraphicalComponent : public Component
{
public:
	GraphicalComponent() {};
	~GraphicalComponent() {};

public:
	bool Initialize() override;
	bool Update(Entity& entity, float dt) override;

	void SetSceneFilepath(const char* filepath);
	void SetTextureFilepath(const char* filepath);

	void SetShader(GLuint shaderID);
	void SetUniformCallback(std::function <void(GraphicsObject* gob, ShaderProgram* shader)> callback);

	void BuildModelToWorldMatrix(glm::vec3 position, glm::mat4 rotate, glm::vec3 scale);

private:
	char m_sceneFilepath[256];
	char m_textureFilepath[256];

	GLuint m_shaderID;
	GraphicsObject m_gob;
};
#endif // !GRAPHICALCOMPONENT_H_
