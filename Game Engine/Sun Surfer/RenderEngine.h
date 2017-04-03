#ifndef RENDERENGINE_H_
#define RENDERENGINE_H_

#pragma warning(push)
#pragma warning(disable: 4201)
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

#include <vector>

#include "SceneLoader.h"
#include "TextureLoader.h"
#include "ShaderProgram.h"
#include "VertexFormat.h"
#include "Light.h"
#include "UniformCallback.h"

#include "ConfigReader.h"

enum Shader
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

class Mesh;
class GraphicsObject;
class MyWindow;
class RenderEngine
{
public:
	static bool Initialize(MyWindow* window, ConfigReader* cfg);
	static bool Shutdown();

	static bool Draw(glm::mat4 worldToViewMat, glm::mat4 projectionMat);
	static bool DrawScene(glm::mat4 worldToViewMat, glm::mat4 viewToProjectionMat);
	static bool DrawScene(glm::mat4 worldToViewMat, glm::mat4 viewToProjectionMat, GLuint shaderID, GLenum drawMode);

	static bool AddGraphicalObject(GraphicsObject* gob);
	static bool AddLight(Light* light);
	static bool AddMesh(Mesh* mesh);

	static bool LoadMeshFromSceneFile(const char* filepath, GraphicsObject * gob, GLuint shaderID);
	static const TextureLoader::Texture* const LoadTextureFromFile(const char* imagepath);

	static GraphicsObject* GetNearestObjectToPosition(glm::vec3 position);

private:
	static bool InitializeShaders();
	static bool ShutdownShaders();

	static void CreateFramebuffer();
	static void CreateShadowFrameBuffer();

	static void SetLightingUniforms(GraphicsObject* gob, ShaderProgram* shader);
	static std::string GetLightPropertyName(const char* propertyName, size_t lightIndex);

private:
	static int		 lightCounter;
	static const int MAX_LIGHTS = 20;
	static Light*	 m_lights[MAX_LIGHTS];

	static GLint currentShaderID;
	static const int maxShaders = 15;
	static ShaderProgram m_shaders[NumShaders];

	static GLuint m_shadowFrameBuffer;
	static GLuint m_depthRenderBuffer;
	static GLuint m_ambientRenderBuffer;
	static const TextureLoader::Texture* m_diffuseAndSpecularTexture;

	static ConfigReader* m_cfg;
	static MyWindow* m_window;
};
#endif // !RENDERENGINE_H_
