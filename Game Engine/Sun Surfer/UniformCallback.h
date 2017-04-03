#ifndef UNIFORMCALLBACK_H_
#define UNIFORMCALLBACK_H_

#include "GraphicsObject.h"
#include "ConfigReader.h"
#include <ctime>

class UniformCallback
{
public:
	static void Debug(GraphicsObject * gob, ShaderProgram * shader);
	static void LitCharacter(GraphicsObject * gob, ShaderProgram * shader);
	static void LitDargon(GraphicsObject * gob, ShaderProgram * shader);

	static void MultipleShading(GraphicsObject * gob, ShaderProgram * shader);
	static void TextureLighting(GraphicsObject * gob, ShaderProgram * shader);
	static void MultipleTextures(GraphicsObject * gob, ShaderProgram * shader);
	static void RenderTexture(GraphicsObject * gob, ShaderProgram * shader);
	static void Billboard(GraphicsObject * gob, ShaderProgram * shader);
	static void Wireframe(GraphicsObject* gob, ShaderProgram* shader);
	static void Explode(GraphicsObject* gob, ShaderProgram* shader);

private:
	static void SetMaterialUniforms(GraphicsObject* gob, ShaderProgram* shader);
	static void SetEmissiveUniforms(GraphicsObject* gob, ShaderProgram* shader);
	static void SetAmbientUniforms(GraphicsObject* gob, ShaderProgram* shader);
	static void SetDiffuseUniforms(GraphicsObject* gob, ShaderProgram* shader);
	static void SetSpecularUniforms(GraphicsObject* gob, ShaderProgram* shader);

	static void BindTextures(GraphicsObject* gob);

private:
	static ConfigReader* cfg;
	static std::clock_t start;
};

#endif // !UNIFORMCALLBACK_H_
