#include "SFMLGraphics.h"

#include "ITextureAtlas.h"

SFMLGraphics::SFMLGraphics(sf::RenderTarget& window, std::shared_ptr<ITextureAtlas> atlas)
	: window(window), atlas(atlas), font(std::make_unique<sf::Font>())
{
	font->loadFromFile("assets/8bit.ttf");
}

void SFMLGraphics::DrawRectangle(sf::Vector2f& position, sf::Vector2f& size, const sf::Color color, const sf::Color outline) const
{
	sf::RectangleShape rectangle;
	rectangle.setFillColor(color);
	rectangle.setOutlineColor(outline);
	rectangle.setPosition(position);
	rectangle.setSize(size);
	this->window.draw(rectangle);
}

void SFMLGraphics::DrawSprite(sf::Vector2f& position, std::string texture, float scale) const
{
	sf::Sprite sprite;
	sprite.scale(scale, scale);
	sprite.setTexture(*this->atlas->GetTexture(texture));
	sprite.setPosition(position);
	this->window.draw(sprite);
}

void SFMLGraphics::DrawText(sf::Vector2f& position, const sf::Color color, unsigned int size, std::string str) const
{
	sf::Text text;
	text.setFont(*this->font);
	text.setCharacterSize(size);
	text.setPosition(position);
	text.setFillColor(color);
	text.setString(str);
	this->window.draw(text);
}

float SFMLGraphics::GetTextureWidth(std::string texture) const
{
	return this->atlas->GetTexture(texture)->getSize().x;
}

float SFMLGraphics::GetTextureHeight(std::string texture) const
{
	return this->atlas->GetTexture(texture)->getSize().y;
}