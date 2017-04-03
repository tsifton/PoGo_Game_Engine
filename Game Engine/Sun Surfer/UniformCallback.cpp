#include "UniformCallback.h"
#include "MyGL.h"
#include "SunSurferGame.h"


ConfigReader* UniformCallback::cfg = ConfigReader::GetInstance();
std::clock_t UniformCallback::start = std::clock();

void UniformCallback::Debug(GraphicsObject * gob, ShaderProgram * shader)
{
	GLint uniformIndex = -1;
	if ((uniformIndex = shader->GetUniformLocation("uEmissiveColor")) != -1) glUniform3fv(uniformIndex, 1, &gob->GetMaterialEmissiveColor()[0]);
}

void UniformCallback::LitCharacter(GraphicsObject * /*gob*/, ShaderProgram * /*shader*/)
{
	// DELETE ASAP
}

void UniformCallback::LitDargon(GraphicsObject * /*gob*/, ShaderProgram * /*shader*/)
{
	// DELETE ASAP
}

void UniformCallback::MultipleShading(GraphicsObject * gob, ShaderProgram * shader)
{
	SetMaterialUniforms(gob, shader);
}

void UniformCallback::TextureLighting(GraphicsObject * gob, ShaderProgram * shader)
{
	SetEmissiveUniforms(gob, shader);
	SetSpecularUniforms(gob, shader);

	BindTextures(gob);
}

void UniformCallback::MultipleTextures(GraphicsObject * gob, ShaderProgram * shader)
{
	SetEmissiveUniforms(gob, shader);
	SetSpecularUniforms(gob, shader);

	BindTextures(gob);
}

void UniformCallback::RenderTexture(GraphicsObject * gob, ShaderProgram * shader)
{
	SetEmissiveUniforms(gob, shader);
	SetSpecularUniforms(gob, shader);

	BindTextures(gob);
}

void UniformCallback::Billboard(GraphicsObject * gob, ShaderProgram * shader)
{
	SetEmissiveUniforms(gob, shader);
	SetSpecularUniforms(gob, shader);

	BindTextures(gob);

	GLint uniformIndex = -1;
	if ((uniformIndex = shader->GetUniformLocation("uhalfWidth")) != -1) glUniform1f(uniformIndex, 2);
}

void UniformCallback::Wireframe(GraphicsObject * gob, ShaderProgram * shader)
{
	SetMaterialUniforms(gob, shader);

	glm::mat4 viewportMat = glm::mat4(960, 0, 0, 960,
									  0, 540, 0, 540,
									  0, 0, 499.9, 500.1,
									  0, 0, 0, 1);

	GLint uniformIndex = -1;
	if ((uniformIndex = shader->GetUniformLocation("uViewportMat")) != -1) glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, &viewportMat[0][0]);
	if ((uniformIndex = shader->GetUniformLocation("line.width")) != -1) glUniform1f(uniformIndex, .5);
	if ((uniformIndex = shader->GetUniformLocation("line.color")) != -1) glUniform4fv(uniformIndex, 1, &glm::vec4(0)[0]);
}

void UniformCallback::Explode(GraphicsObject * gob, ShaderProgram * shader)
{
	SetEmissiveUniforms(gob, shader);
	SetSpecularUniforms(gob, shader);

	BindTextures(gob);

	glm::mat4 viewportMat = glm::mat4(960, 0, 0, 960,
									  0, 540, 0, 540,
									  0, 0, 499.9, 500.1,
									  0, 0, 0, 1);

	GLint uniformIndex = -1;
	if ((uniformIndex = shader->GetUniformLocation("time")) != -1) glUniform1f(uniformIndex, static_cast<float>(std::clock()));
	if ((uniformIndex = shader->GetUniformLocation("uViewportMat")) != -1) glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, &viewportMat[0][0]);
	if ((uniformIndex = shader->GetUniformLocation("line.width")) != -1) glUniform1f(uniformIndex, .5);
	if ((uniformIndex = shader->GetUniformLocation("line.color")) != -1) glUniform4fv(uniformIndex, 1, &glm::vec4(0)[0]);
}

void UniformCallback::SetMaterialUniforms(GraphicsObject * gob, ShaderProgram * shader)
{
	SetEmissiveUniforms(gob, shader);
	SetAmbientUniforms(gob, shader);
	SetDiffuseUniforms(gob, shader);
	SetSpecularUniforms(gob, shader);
}

void UniformCallback::SetEmissiveUniforms(GraphicsObject * gob, ShaderProgram * shader)
{
	GLint uniformIndex = -1;
	if ((uniformIndex = shader->GetUniformLocation("uEmissiveColor")) != -1) glUniform3fv(uniformIndex, 1, &gob->GetMaterialEmissiveColor()[0]);
}

void UniformCallback::SetAmbientUniforms(GraphicsObject * gob, ShaderProgram * shader)
{
	GLint uniformIndex = -1;
	if ((uniformIndex = shader->GetUniformLocation("material.ambient")) != -1) glUniform3fv(uniformIndex, 1, &gob->GetMaterialAmbientColor()[0]);
}

void UniformCallback::SetDiffuseUniforms(GraphicsObject * gob, ShaderProgram * shader)
{
	GLint uniformIndex = -1;
	if ((uniformIndex = shader->GetUniformLocation("material.diffuse")) != -1) glUniform3fv(uniformIndex, 1, &gob->GetMaterialDiffuseColor()[0]);
}

void UniformCallback::SetSpecularUniforms(GraphicsObject * gob, ShaderProgram * shader)
{
	GLint uniformIndex = -1;
	if ((uniformIndex = shader->GetUniformLocation("material.specular")) != -1) glUniform3fv(uniformIndex, 1, &gob->GetMaterialSpecularColor()[0]);
	if ((uniformIndex = shader->GetUniformLocation("material.shininess")) != -1) glUniform1f(uniformIndex, gob->GetMaterialShininessFactor());
}

void UniformCallback::BindTextures(GraphicsObject * gob)
{
	std::vector<const TextureLoader::Texture*> textures = gob->GetTextures();
	for (size_t i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->textureID);
	}
}
