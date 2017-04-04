#ifndef RENDERENGINE_H_
#define RENDERENGINE_H_

#pragma warning(push)
#pragma warning(disable: 4201)
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

#include "SceneLoader.h"
#include "ShaderProgram.h"
#include "VertexFormat.h"
#include "Light.h"
#include "UniformCallback.h"
#include "ConfigReader.h"

struct ShaderType
{
	enum
	{
		PassThrough,
		Debug,
		Texture,
		MultiplePhong,
		MultipleTextures,
		TextureLighting,
		DepthOnly,
		PhongShadow,
		PhongShadowVolumes,
		ShadowVolumeGeneration,
		FullscreenQuad,

		NumShaders // always last
	};
};

// Foward Declarations
class MyWindow;
class GraphicsObject;
class Mesh;

class RenderEngine
{
public:
	bool Draw(glm::mat4 worldToViewMat, glm::mat4 projectionMat);
	bool DrawScene(glm::mat4 worldToViewMat, glm::mat4 viewToProjectionMat);
	bool DrawScene(glm::mat4 worldToViewMat, glm::mat4 viewToProjectionMat, GLuint shaderID, GLenum drawMode);

	bool AddGraphicalObject(GraphicsObject* gob);
	bool AddLight(Light* light);
	bool AddMesh(Mesh* mesh);

	bool LoadMeshFromSceneFile(const char* filepath, GraphicsObject * gob, GLuint shaderID);
	const Texture* const LoadTextureFromFile(const char* imagepath);

	GraphicsObject* GetNearestObjectToPosition(glm::vec3 position);

private:
	RenderEngine() {};

	bool Initialize(MyWindow* window, ConfigReader* cfg);
	bool Shutdown();

	bool InitializeShaders();
	bool ShutdownShaders();

	void CreateFramebuffer();
	void CreateShadowFrameBuffer();

	void SetLightingUniforms(GraphicsObject* gob, ShaderProgram* shader);
	std::string GetLightPropertyName(const char* propertyName, size_t lightIndex);

	int lightCounter;
	static const int MAX_LIGHTS = 20;
	Light* m_lights[MAX_LIGHTS];

	GLint currentShaderID;
	const int maxShaders = 15;
	ShaderProgram m_shaders[ShaderType::NumShaders];

	GLuint m_shadowFrameBuffer;
	GLuint m_depthRenderBuffer;
	GLuint m_ambientRenderBuffer;
	const Texture* m_diffuseAndSpecularTexture;

	ConfigReader* m_cfg;
	MyWindow* m_window;
};

#endif // !RENDERENGINE_H_
