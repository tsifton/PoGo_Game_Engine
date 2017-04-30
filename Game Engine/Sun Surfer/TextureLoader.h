#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_

#include <map>
#include <memory>

#include "GL\glew.h"
#include "Texture.h"

class TextureLoader
{
public:
	static TextureLoader& Instance();

	TextureLoader(const TextureLoader&)  = delete;
	void operator=(const TextureLoader&) = delete;

	~TextureLoader() {};

	Texture::SharedPtr GetTexture(const std::string& imagepath);
	Texture::SharedPtr GetEmptyTexture(int width, int height);
	Texture::SharedPtr GetEmptyShadowMapTexture(int width, int height);

private:
	typedef std::unique_ptr<const Texture>			TextureUniquePtr;
	typedef std::weak_ptr<const Texture>			TextureWeakPtr;
	typedef std::map<std::string, TextureWeakPtr>	TextureMap;

	TextureLoader() {};

	Texture::SharedPtr GetLoadedTexture(const std::string& imagepath);
	Texture::SharedPtr LoadTexture(const std::string& imagepath);
	Texture::SharedPtr BufferTexture(const std::string& imagepath, GLuint id, int width, int height);

	TextureMap m_textureMap;
};
#endif // !TEXTURELOADER_H_
