#include "RenderEngine.h"
#include "GameLogger.h"

#include "BufferManager.h"
#include "GraphicsObject.h"
#include "Mesh.h"
#include "MyWindow.h"
#include "MyGL.h"

#include <sstream>

bool RenderEngine::Initialize()
{
	if (!BufferManager::Initialize()) return false;
	if (!InitializeShaders()) return false;

	GameLogger::Log(MsgType::Process, "RenderEngine::Initialize() successful.\n");
	return true;
}

bool RenderEngine::Shutdown()
{
	if (!ShutdownShaders()) return false;
	if (!BufferManager::Shutdown()) return false;

	GameLogger::Log(MsgType::Process, "RenderEngine::Shutdown() successful.\n");
	return true;
}

bool RenderEngine::Draw(glm::mat4 worldToViewMat, glm::mat4 viewToProjectionMat)
{
	DrawScene(worldToViewMat, viewToProjectionMat);
	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	//glDisable(GL_STENCIL_TEST);

	////first pass draw scene normally, outputting color data to the ambient buffer and diffuse+specular to a texture
	//glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFrameBuffer);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//DrawScene(worldToViewMat, viewToProjectionMat);

	//// Second Pass
	//const GLuint     unbindFbo = 0; // 0 unbinds fbo
	//GLint      srcX0 = 0;
	//GLint      srcY0 = 0;
	//GLint      srcX1 = 1920;
	//GLint      srcY1 = 1080;
	//GLint      destX0 = 0;
	//GLint      destY0 = 0;
	//GLint      destX1 = 1920;
	//GLint      destY1 = 1080;
	//const GLbitfield mask = GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT;
	//const GLenum     filter = GL_NEAREST;

	//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_shadowFrameBuffer);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, unbindFbo);
	//glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, destX0, destY0, destX1, destY1, mask, filter);

	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//glDepthMask(GL_FALSE);

	//glBindFramebuffer(GL_FRAMEBUFFER, unbindFbo);

	//const GLenum stencilFunc = GL_ALWAYS;
	//const GLint  stencilRefValue = 0;
	//const GLuint stencilMask = 0xFFFF;
	//const GLenum stencilFailAction = GL_KEEP;
	//const GLenum depthFailAction = GL_KEEP;
	//const GLenum frontPassAction = GL_INCR_WRAP;
	//const GLenum backPassAction = GL_DECR_WRAP;

	//glClear(GL_STENCIL_BUFFER_BIT);
	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(stencilFunc, stencilRefValue, stencilMask);
	//glStencilOpSeparate(GL_FRONT, stencilFailAction, depthFailAction, frontPassAction);
	//glStencilOpSeparate(GL_BACK, stencilFailAction, depthFailAction, backPassAction);

	//DrawScene(worldToViewMat, viewToProjectionMat, ShaderType::ShadowVolumeGeneration, GL_TRIANGLES);

	//// Pass Three
	//glDisable(GL_DEPTH_TEST);
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	//const GLenum stencilFunc2 = GL_EQUAL;
	//const GLint  stencilRefValue2 = 0;
	//const GLuint stencilMask2 = 0xFFFF;
	//const GLenum failAction = GL_KEEP;
	//const GLenum zFailAction = GL_KEEP;
	//const GLenum zPassAction = GL_KEEP;

	//glStencilFunc(stencilFunc2, stencilRefValue2, stencilMask2);
	//glStencilOp(failAction, zFailAction, zPassAction);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_diffuseAndSpecularTexture->id);
	//m_shaders[ShaderType::FullscreenQuad].UseProgram();
	//glDrawArrays(GL_POINTS, 0, 1);

	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	return true;
}

bool RenderEngine::DrawScene(glm::mat4 worldToViewMat, glm::mat4 viewToProjectionMat)
{
	const int numBufferGroups = BufferManager::GetNumBufferGroups();
	BufferGroup* bufferGroups = BufferManager::GetBufferGroups();

	for (int i = 0; i < numBufferGroups; i++)
	{
		BufferGroup& bufferGroup = bufferGroups[i];
		GLint shaderID = bufferGroup.shaderID;

		ShaderProgram& currentShader = m_shaders[shaderID];
		if (shaderID != currentShaderID)
		{
			currentShader.UseProgram();
			currentShaderID = shaderID;
		}

		const int numBuffers = bufferGroup.bufferCounter;
		for (int j = 0; j < numBuffers; j++)
		{
			Buffer& buffer = bufferGroup.m_buffers[j];
			glBindVertexArray(buffer.vertArrayObjectID);

			GLint uniformIndex = -1;
			if ((uniformIndex = currentShader.GetUniformLocation("uViewMat")) != -1)		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, &worldToViewMat[0][0]);
			if ((uniformIndex = currentShader.GetUniformLocation("uProjectionMat")) != -1)	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, &viewToProjectionMat[0][0]);

			for (GraphicsObject* gob = buffer.headNode; gob != nullptr && gob->isEnabled; gob = gob->nextNode)
			{
				Mesh* mesh = gob->mesh;
				RenderInfo info = mesh->renderInfo;

				if (gob->GetUniformCallback()) gob->GetUniformCallback()(gob, &currentShader);

				glm::mat4 modelToWorldMat = gob->GetModelToWorldMatrix();
				glm::mat4 MVP = viewToProjectionMat * (worldToViewMat * modelToWorldMat);

				SetLightingUniforms(gob, &currentShader);
				if ((uniformIndex = currentShader.GetUniformLocation("uModelMat")) != -1) glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, &modelToWorldMat[0][0]);
				if ((uniformIndex = currentShader.GetUniformLocation("uMVP")) != -1)	  glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, &MVP[0][0]);

				if (mesh->numIndices == 0) glDrawArrays(info.mode, info.vertOffset, mesh->numVerts);
				else glDrawElementsBaseVertex(info.mode, mesh->numIndices, static_cast<int>(info.indexDataType), (void*)info.indexOffsetInBytes, info.vertOffset);
			}
		}
	}
	return true;
}

bool RenderEngine::DrawScene(glm::mat4 worldToViewMat, glm::mat4 viewToProjectionMat, GLuint shaderID, GLenum drawMode)
{
	ShaderProgram& currentShader = m_shaders[shaderID];
	currentShader.UseProgram();

	const int numBufferGroups = BufferManager::GetNumBufferGroups();
	BufferGroup* bufferGroups = BufferManager::GetBufferGroups();

	for (int i = 0; i < numBufferGroups; i++)
	{
		BufferGroup& bufferGroup = bufferGroups[i];

		const int numBuffers = bufferGroup.bufferCounter;
		for (int j = 0; j < numBuffers; j++)
		{
			Buffer& buffer = bufferGroup.m_buffers[j];
			glBindVertexArray(buffer.vertArrayObjectID);

			GLint uniformIndex = -1;

			if ((uniformIndex = currentShader.GetUniformLocation("uViewMat")) != -1)		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, &worldToViewMat[0][0]);
			if ((uniformIndex = currentShader.GetUniformLocation("uProjectionMat")) != -1)	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, &viewToProjectionMat[0][0]);

			for (GraphicsObject* gob = buffer.headNode; gob != nullptr && gob->isEnabled; gob = gob->nextNode)
			{
				Mesh* mesh = gob->mesh;
				RenderInfo info = mesh->renderInfo;

				glm::mat4 modelToWorldMat = gob->GetModelToWorldMatrix();
				glm::mat4 MVP = viewToProjectionMat * (worldToViewMat * modelToWorldMat);

				if ((uniformIndex = currentShader.GetUniformLocation("uModelMat")) != -1) glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, &modelToWorldMat[0][0]);
				if ((uniformIndex = currentShader.GetUniformLocation("uMVP")) != -1)	  glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, &MVP[0][0]);

				if (mesh->numIndices == 0) glDrawArrays(drawMode, info.vertOffset, mesh->numVerts);
				else glDrawElementsBaseVertex(drawMode, mesh->numIndices, static_cast<int>(info.indexDataType), (void*)info.indexOffsetInBytes, info.vertOffset);
			}
		}
	}
	return true;
}

void RenderEngine::SetLightingUniforms(GraphicsObject * gob, ShaderProgram * shader)
{
	std::vector<int> lightIndices = gob->material.GetLightIndices();
	int numIndices = lightIndices.size();

	if (numIndices > 0)
	{
		GLint uniformIndex = -1;
		int lightsUsed = 0;
		for (unsigned int i = 0; i < numIndices; ++i) {
			int lightIndex = lightIndices[i];
			if (lightIndex < lightCounter)
			{
				std::string propertyName;
				Light* light = m_lights[lightIndex];

				propertyName = GetLightPropertyName("position", lightsUsed);
				if ((uniformIndex = shader->GetUniformLocation(propertyName.c_str())) != -1) glUniform4fv(uniformIndex, 1, &light->GetPosition()[0]);

				propertyName = GetLightPropertyName("ambient", lightsUsed);
				if ((uniformIndex = shader->GetUniformLocation(propertyName.c_str())) != -1) glUniform3fv(uniformIndex, 1, &light->GetAmbientColor()[0]);

				propertyName = GetLightPropertyName("diffuse", lightsUsed);
				if ((uniformIndex = shader->GetUniformLocation(propertyName.c_str())) != -1) glUniform3fv(uniformIndex, 1, &light->GetDiffuseColor()[0]);

				propertyName = GetLightPropertyName("specular", lightsUsed);
				if ((uniformIndex = shader->GetUniformLocation(propertyName.c_str())) != -1) glUniform3fv(uniformIndex, 1, &light->GetSpecularColor()[0]);

				propertyName = GetLightPropertyName("direction", lightsUsed);
				if ((uniformIndex = shader->GetUniformLocation(propertyName.c_str())) != -1) glUniform3fv(uniformIndex, 1, &glm::normalize(light->GetDirection())[0]);

				propertyName = GetLightPropertyName("attenuation", lightsUsed);
				if ((uniformIndex = shader->GetUniformLocation(propertyName.c_str())) != -1) glUniform1f(uniformIndex, light->GetAttenuation());

				propertyName = GetLightPropertyName("cutoffAngle", lightsUsed);
				if ((uniformIndex = shader->GetUniformLocation(propertyName.c_str())) != -1) glUniform1f(uniformIndex, light->GetCutoffAngle());

				lightsUsed++;
			}
		}

		if ((uniformIndex = shader->GetUniformLocation("numLights")) != -1) glUniform1iv(uniformIndex, 1, &lightsUsed);
	}
}

std::string RenderEngine::GetLightPropertyName(const std::string& propertyName, unsigned int lightIndex)
{
	std::ostringstream ss;
	ss << "lights[" << lightIndex << "]." << propertyName;
	return ss.str();
}

bool RenderEngine::AddGraphicalObject(GraphicsObject * gob)
{
	return BufferManager::AddGraphicalObject(gob);
}

bool RenderEngine::AddLight(Light * light)
{
	if (lightCounter < MAX_LIGHTS)
	{
		m_lights[lightCounter++] = light;
		AddGraphicalObject(light);
	}
	return true;
}

bool RenderEngine::AddMesh(Mesh * mesh)
{
	return BufferManager::AddMesh(mesh);
}

bool RenderEngine::LoadMeshFromSceneFile(const char * filepath, GraphicsObject * gob, GLuint shaderID)
{
	gob->shaderID = shaderID;
	if (SceneLoader::GetMeshFromSceneFile(filepath, gob->mesh))
	{
		gob->mesh->shaderID = shaderID;
		return AddMesh(gob->mesh);
	}
	return false;
}

Texture::SharedPtr RenderEngine::LoadTextureFromFile(const std::string & imagepath)
{
	return m_textureLoader.GetTexture(imagepath);
}

GraphicsObject* RenderEngine::GetNearestObjectToPosition(const glm::vec3& position)
{
	float smallestDistance = -1.0f;
	GraphicsObject* nearestObject = nullptr;

	const int numBufferGroups = BufferManager::GetNumBufferGroups();
	BufferGroup* bufferGroups = BufferManager::GetBufferGroups();
	for (int i = 0; i < numBufferGroups; i++)
	{
		BufferGroup& bufferGroup = bufferGroups[i];

		const int numBuffers = bufferGroup.bufferCounter;
		for (int j = 0; j < numBuffers; j++)
		{
			Buffer& buffer = bufferGroup.m_buffers[j];

			for (GraphicsObject* gob = buffer.headNode; gob != nullptr && gob->isEnabled; gob = gob->nextNode)
			{
				glm::vec3 positionOfObject = glm::vec3(gob->GetPosition() * glm::vec4(0, 0, 0, 1));
				glm::vec3 distanceVec = position - positionOfObject;
				float distance = sqrtf(powf(distanceVec.x, 2) + powf(distanceVec.y, 2) + powf(distanceVec.z, 2));
				if (distance < smallestDistance || smallestDistance == -1.0f)
				{
					smallestDistance = distance;
					nearestObject = gob;
				}
			}
		}
	}
	return nearestObject;
}

bool RenderEngine::InitializeShaders()
{
	if (!m_shaders[ShaderType::PassThrough].Initialize()) return false;
	if (!m_shaders[ShaderType::PassThrough].AddVertexShader("PassThrough.vert.shader")) return false;
	if (!m_shaders[ShaderType::PassThrough].AddFragmentShader("PassThrough.frag.shader")) return false;
	if (!m_shaders[ShaderType::PassThrough].LinkProgram()) return false;

	if (!m_shaders[ShaderType::Debug].Initialize()) return false;
	if (!m_shaders[ShaderType::Debug].AddVertexShader("Debug.vert.shader")) return false;
	if (!m_shaders[ShaderType::Debug].AddFragmentShader("Debug.frag.shader")) return false;
	if (!m_shaders[ShaderType::Debug].LinkProgram()) return false;

	if (!m_shaders[ShaderType::MultiplePhong].Initialize()) return false;
	if (!m_shaders[ShaderType::MultiplePhong].AddVertexShader("MultiplePhong.vert.shader")) return false;
	if (!m_shaders[ShaderType::MultiplePhong].AddFragmentShader("MultiplePhong.frag.shader")) return false;
	if (!m_shaders[ShaderType::MultiplePhong].LinkProgram()) return false;

	if (!m_shaders[ShaderType::TextureLighting].Initialize()) return false;
	if (!m_shaders[ShaderType::TextureLighting].AddVertexShader("SimpleTexture.vert.shader")) return false;
	if (!m_shaders[ShaderType::TextureLighting].AddFragmentShader("SimpleTexture.frag.shader")) return false;
	if (!m_shaders[ShaderType::TextureLighting].LinkProgram()) return false;

	if (!m_shaders[ShaderType::MultipleTextures].Initialize()) return false;
	if (!m_shaders[ShaderType::MultipleTextures].AddVertexShader("MultipleTextures.vert.shader")) return false;
	if (!m_shaders[ShaderType::MultipleTextures].AddFragmentShader("MultipleTextures.frag.shader")) return false;
	if (!m_shaders[ShaderType::MultipleTextures].LinkProgram()) return false;

	if (!m_shaders[ShaderType::DepthOnly].Initialize()) return false;
	if (!m_shaders[ShaderType::DepthOnly].AddVertexShader("DepthOnly.vert.shader")) return false;
	if (!m_shaders[ShaderType::DepthOnly].AddFragmentShader("DepthOnly.frag.shader")) return false;
	if (!m_shaders[ShaderType::DepthOnly].LinkProgram()) return false;

	if (!m_shaders[ShaderType::PhongShadow].Initialize()) return false;
	if (!m_shaders[ShaderType::PhongShadow].AddVertexShader("PhongShadow.vert.shader")) return false;
	if (!m_shaders[ShaderType::PhongShadow].AddFragmentShader("PhongShadow.frag.shader")) return false;
	if (!m_shaders[ShaderType::PhongShadow].LinkProgram()) return false;

	if (!m_shaders[ShaderType::PhongShadowVolumes].Initialize()) return false;
	if (!m_shaders[ShaderType::PhongShadowVolumes].AddVertexShader("PhongShadowVolumes.vert.shader")) return false;
	if (!m_shaders[ShaderType::PhongShadowVolumes].AddFragmentShader("PhongShadowVolumes.frag.shader")) return false;
	if (!m_shaders[ShaderType::PhongShadowVolumes].LinkProgram()) return false;

	if (!m_shaders[ShaderType::ShadowVolumeGeneration].Initialize()) return false;
	if (!m_shaders[ShaderType::ShadowVolumeGeneration].AddVertexShader("ShadowVolumeGeneration.vert.shader")) return false;
	if (!m_shaders[ShaderType::ShadowVolumeGeneration].AddGeometryShader("ShadowVolumeGeneration.geom.shader")) return false;
	if (!m_shaders[ShaderType::ShadowVolumeGeneration].AddFragmentShader("ShadowVolumeGeneration.frag.shader")) return false;
	if (!m_shaders[ShaderType::ShadowVolumeGeneration].LinkProgram()) return false;

	if (!m_shaders[ShaderType::FullscreenQuad].Initialize()) return false;
	if (!m_shaders[ShaderType::FullscreenQuad].AddVertexShader("FullscreenQuad.vert.shader")) return false;
	if (!m_shaders[ShaderType::FullscreenQuad].AddGeometryShader("FullscreenQuad.geom.shader")) return false;
	if (!m_shaders[ShaderType::FullscreenQuad].AddFragmentShader("FullscreenQuad.frag.shader")) return false;
	if (!m_shaders[ShaderType::FullscreenQuad].LinkProgram()) return false;

	return true;
}

bool RenderEngine::ShutdownShaders()
{
	for (int i = 0; i < ShaderType::NumShaders; i++)
	{
		if (!m_shaders[i].Shutdown()) return false;
	}
	return true;
}

void RenderEngine::CreateFramebuffer()
{
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		GameLogger::Log(MsgType::Error, "Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderEngine::CreateShadowFrameBuffer()
{
	m_diffuseAndSpecularTexture = m_textureLoader.GetEmptyTexture(1920, 1080);

	glGenRenderbuffers(1, &m_depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1920, 1080);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenRenderbuffers(1, &m_ambientRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ambientRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, 1920, 1080);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &m_shadowFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFrameBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ambientRenderBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_diffuseAndSpecularTexture->id, 0);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	const GLsizei numDrawBuffers = sizeof(drawBuffers) / sizeof(drawBuffers[0]);
	glDrawBuffers(numDrawBuffers, drawBuffers);

	bool isCompleteFramebuffer = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
