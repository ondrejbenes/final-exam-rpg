#include "Minimap.h"
#include "EntityManager.h"
#include "Tilemap.h"
#include "MainGame.h"

Minimap::Minimap(sf::Texture* texture)
{
	_image.setTexture(*texture);
	_imageWidth = _image.getGlobalBounds().width;
	_imageHeight = _image.getGlobalBounds().height;

	_visibleArea = sf::RectangleShape();
	_visibleArea.setFillColor(sf::Color::Transparent);
	_visibleArea.setOutlineColor(sf::Color::Black);
	_visibleArea.setOutlineThickness(1);

	_playerPosition = sf::CircleShape(2);
	_playerPosition.setFillColor(sf::Color::Magenta);
}

void Minimap::draw(std::shared_ptr<sf::RenderWindow> window) 
{
	auto view = window->getView();
	auto viewLeft = view.getCenter().x - view.getSize().x / 2;
	auto viewRight = view.getCenter().x + view.getSize().x / 2;
	auto viewTop = view.getCenter().y - view.getSize().y / 2;
	auto viewBottom = view.getCenter().y + view.getSize().y / 2;

	auto imageX = viewRight - _imageWidth;
	auto imageY = viewTop;
	_image.setPosition(imageX, imageY);

	auto rectX = imageX + (viewLeft / Tilemap::MAP_WIDTH) * _imageWidth;
	auto rectY = imageY + (viewTop / Tilemap::MAP_HEIGHT) * _imageHeight;
	auto rectWidth = (viewRight - viewLeft) / Tilemap::MAP_WIDTH * _imageWidth;
	auto rectHeight = (viewBottom - viewTop) / Tilemap::MAP_HEIGHT * _imageHeight;
	_visibleArea.setPosition(rectX, rectY);
	_visibleArea.setSize(sf::Vector2f(rectWidth, rectHeight));

	auto player = EntityManager::getInstance()->getLocalPlayer();
	auto dotX = imageX + (player->getPosition().x / Tilemap::MAP_WIDTH) * _imageWidth;
	auto dotY = imageY + (player->getPosition().y / Tilemap::MAP_HEIGHT) * _imageHeight;
	_playerPosition.setPosition(dotX, dotY);

	window->draw(_image);
	window->draw(_visibleArea);
	window->draw(_playerPosition);
}

sf::FloatRect Minimap::getBounds() 
{
	return _image.getGlobalBounds();
}