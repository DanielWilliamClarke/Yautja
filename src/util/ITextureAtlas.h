#ifndef I_TEXTURE_ATLAS
#define I_TEXTURE_ATLAS
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class ITextureAtlas : public std::enable_shared_from_this<ITextureAtlas>
{
public:
	ITextureAtlas() = default;
	virtual ~ITextureAtlas() = default;
	virtual std::shared_ptr<ITextureAtlas> AddTexture(std::string tag, std::string texturePath) = 0;
	virtual std::shared_ptr<sf::Texture> GetTexture(std::string tag) const = 0;
};

#endif // I_TEXTURE_ATLAS