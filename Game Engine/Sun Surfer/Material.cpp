#include "Material.h"

void Material::AddLightIndex(int index)
{
	lightIndices.insert(lightIndices.begin(), index);
}

void Material::AddLightIndices(int indices[], int numValues)
{
	lightIndices.insert(lightIndices.begin(), indices, indices + numValues);
}

void Material::AddTexture(const TextureLoader::Texture* texture)
{
	textures.push_back(texture);
}
