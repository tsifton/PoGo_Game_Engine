#ifndef TEXTURE_H_
#define TEXTURE_H_

struct Texture
{
	Texture(unsigned int id, int width, int height) : handle(id), width(width), height(height) {};

	unsigned int handle;
	int width, height;
};

#endif // !TEXTURE_H_
