#include "Image.h"

Image::Image(sf::Texture* texture) 
{
	_sprite.setTexture(*texture);
}

void Image::draw(std::shared_ptr<sf::RenderWindow> window) 
{
	window->draw(_sprite);
}

sf::FloatRect Image::getBounds() 
{
	return _sprite.getGlobalBounds();
}

void Image::setPosition(const sf::Vector2f& pos) 
{
	_sprite.setPosition(pos);
	UiElement::setPosition(pos);
}

void Image::changeTexture(sf::Texture* newTexture) 
{
	_sprite.setTexture(*newTexture);
}
