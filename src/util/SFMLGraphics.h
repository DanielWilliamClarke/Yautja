#ifndef SFML_GRAPHICS
#define SFML_GRAPHICS
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "IGraphics.h"

class ITextureAtlas;

class SFMLGraphics : public IGraphics
{
public:
	SFMLGraphics(sf::RenderTarget& window, std::shared_ptr<ITextureAtlas> atlas);
	virtual ~SFMLGraphics() = default;

	virtual void DrawRectangle(sf::Vector2f& position, sf::Vector2f& size, const sf::Color color, const sf::Color outline) const override;
	virtual void DrawSprite(sf::Vector2f& position, std::string texture, float scale) const override;
	virtual void DrawText(sf::Vector2f& position, const sf::Color color, unsigned int size, std::string str) const override;
	virtual float GetTextureWidth(std::string texture) const override;
	virtual float GetTextureHeight(std::string texture) const override;
private:
	sf::RenderTarget& window;
	std::shared_ptr<ITextureAtlas> atlas;
	std::unique_ptr<sf::Font> font;
};

#endif // SFML_GRAPHICS