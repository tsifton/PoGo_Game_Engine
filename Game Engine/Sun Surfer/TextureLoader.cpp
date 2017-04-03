#include "TextureLoader.h"
#include "GameLogger.h"

#include <fstream>
#include <vector>
#include "GL\glew.h"

TextureLoader & TextureLoader::Instance()
{
	static TextureLoader* instance = new TextureLoader();
	return *instance;
}

bool TextureLoader::GetTexture(const std::string & imagepath, std::shared_ptr<const Texture> & outTexture)
{
	return GetLoadedTexture(imagepath, outTexture) || LoadTexture(imagepath, outTexture);
}

std::shared_ptr<const Texture> TextureLoader::GetEmptyTexture(const std::string& name, int width, int height)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	return BufferTexture(name, textureID, width, height);
}

std::shared_ptr<const Texture> TextureLoader::GetEmptyShadowMapTexture(const std::string & name, int width, int height)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	
	return BufferTexture(name, textureID, width, height);
}

bool TextureLoader::GetLoadedTexture(const std::string & imagepath, TextureSharedPtr & outTexture)
{
	// Iterate through loaded textures to find matching imagepath
	TextureMap::iterator itr = m_textureMap.find(imagepath);

	// If one is found, set the out texture to the created shared_ptr
	// Return true if the ptr is not nullptr
	if (itr != m_textureMap.end()) return (outTexture = itr->second.lock()).get();

	GameLogger::Log(MsgType::Info, "TextureLoader::GetLoadedTexture() - Unable to find a textureID already loaded with the filepath [%s].\n", imagepath);
	return false;
}

bool TextureLoader::LoadTexture(const std::string & imagepath, TextureSharedPtr & outTexture)
{
	// Data read from the header of the BMP file
	char header[54];		// Each BMP file begins by a 54-bytes header
	unsigned int dataPos;	// Position in the file where the actual data begins
	int width, height;		// Width and Height of image
	int imageSize;			// width * height * 3 (no alpha) or width * height * 4 (alpha)
	
	// Open the file
	std::ifstream ifs(imagepath, std::ios::binary);
	if (!ifs)
	{ 
		GameLogger::Log(MsgType::Error, "TextureLoader::Load_BMP_Texture() - Unable to open textureID file with imagepath [%s].\n", imagepath);
		return false; 
	}
	
	if (!ifs.read(header, 54)) { // If not 54 bytes read : problem
		GameLogger::Log(MsgType::Error, "TextureLoader::Load_BMP_Texture() - The BMP file [%s] does not have a proper 54 byte header.\n", imagepath);
		return false;
	}
	
	if (header[0] != 'B' || header[1] != 'M') {
		GameLogger::Log(MsgType::Error, "TextureLoader::Load_BMP_Texture() - The BMP file [%s] does not start with the required characters B and M.\n", imagepath);
		return false;
	}
	
	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	
	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0) imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)   dataPos = 54; // The BMP header is done that way
	
	// Create a buffer
	std::vector<char> imageBuffer;
	
	// Read the actual data from the file into the buffer
	ifs.read(imageBuffer.data(), imageSize);
	
	//Everything is in memory now, the file can be closed
	ifs.close();
	
	// Create an OpenGL textureID
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created textureID : all future textureID functions will modify this textureID
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	// Check if the image has Alpha Data
	bool hasAlpha = (width * height * 4) == imageSize;
	
	// Give the image to OpenGL
	if (hasAlpha)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, imageBuffer.data());
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageBuffer.data());
	
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/
	
	// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	// Activate anisoptropic filtering, using the max possible (most likely x16)
	float aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
	
	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	
	// Add Texture to Texture Map
	BufferTexture(imagepath, textureID, width, height);

	return true;
}

std::shared_ptr<const Texture> TextureLoader::BufferTexture(const std::string& imagepath, unsigned int id, int width, int height)
{
	TextureSharedPtr sharedTexture(new Texture(id, width, height));
	TextureWeakPtr   weakTexture(sharedTexture);
	m_textureMap.insert(std::make_pair(imagepath, weakTexture));
	return sharedTexture;
}