#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <memory>

struct Texture
{
	typedef std::shared_ptr<const Texture> SharedPtr;

	Texture(unsigned int handle, int width, int height) : id(handle), width(width), height(height) {};

	unsigned int id;
	int width, height;
};

#endif // !TEXTURE_H_
