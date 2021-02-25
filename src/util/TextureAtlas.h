#ifndef TEXTURE_ATLAS
#define TEXTURE_ATLAS
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <map>

#include "ITextureAtlas.h"

class TextureAtlas : public ITextureAtlas
{
public:
	TextureAtlas() = default;
	virtual ~TextureAtlas() = default;
	virtual std::shared_ptr<ITextureAtlas> AddTexture(std::string tag, std::string texturePath) override;
	virtual std::shared_ptr<sf::Texture> GetTexture(std::string tag) const override;

private:
	std::map<std::string, std::shared_ptr<sf::Texture>> textures;
};

#endif // TEXTURE_ATLAS