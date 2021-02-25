#include "TextureAtlas.h"

std::shared_ptr<ITextureAtlas> TextureAtlas::AddTexture(std::string tag, std::string texturePath)
{
	auto texture = std::make_shared<sf::Texture>();
	texture->loadFromFile(texturePath);
	this->textures[tag] = texture;
	return shared_from_this();
}

std::shared_ptr<sf::Texture> TextureAtlas::GetTexture(std::string tag) const
{
	return this->textures.at(tag);
}