#ifndef I_GRAPHICS
#define I_GRAPHICS
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class IGraphics
{
public:
	IGraphics() = default;
	virtual ~IGraphics() = default;

	virtual void DrawRectangle(sf::Vector2f& position, sf::Vector2f& size, const sf::Color color, const sf::Color outline = sf::Color::Transparent) const = 0;
	virtual void DrawSprite(sf::Vector2f& position, std::string texture, float scale = 1.0f) const = 0;
	virtual void DrawText(sf::Vector2f& position, const sf::Color color, unsigned int size, std::string text) const = 0;
	virtual float GetTextureWidth(std::string texture) const = 0;
	virtual float GetTextureHeight(std::string texture) const = 0;
};

#endif // I_GRAPHICS