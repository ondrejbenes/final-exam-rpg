#include "Inventory.h"
#include "EntityManager.h"
#include "GraphicsComponent.h"

Inventory::Inventory()
{
	_background = sf::RectangleShape(sf::Vector2f(width, height));

	auto inventoryLambda = [](UiElement* source, const sf::Event& event)
	{
		auto inventory = dynamic_cast<Inventory*>(source);
		auto bounds = inventory->getBounds();

		auto x = event.mouseButton.x - bounds.left;
		auto y = event.mouseButton.y - bounds.top;

		auto column = floor(x / (width / iconsPerRow));
		auto row = floor(y / (height / (maxItems / iconsPerRow)));

		auto itemToHighlight = unsigned(row * iconsPerRow + column);

		auto player = EntityManager::getInstance()->getLocalPlayer();
		auto playerInventory = player->getInventory();

		if (playerInventory.size() <= itemToHighlight)
			return;

		auto item = playerInventory[itemToHighlight];

		if (typeid(*item) == typeid(Weapon))
		{
			inventory->setHighlightedItem(itemToHighlight);
			player->setEquipedWeapon(std::dynamic_pointer_cast<Weapon>(item));
		}
	};
	
	setOnClick(new UiCallback(inventoryLambda));
}

void Inventory::draw(std::shared_ptr<sf::RenderWindow> window) 
{
	auto rectPos = window->mapPixelToCoords(sf::Vector2i(0, 0));

	_background.setPosition(rectPos);

	if (isFocused())
		_background.setFillColor(sf::Color::Black);
	else
		_background.setFillColor(sf::Color(0, 0, 0, 32));

	window->draw(_background);

	auto playerInventory = EntityManager::getInstance()->getLocalPlayer()->getInventory();

	auto x = rectPos.x + padding;
	auto y = rectPos.y + padding;
	auto itemCount = 0;

	for(auto it = begin(playerInventory); it != end(playerInventory); ++it)
	{
		auto sprite = (*it)->getComponent<GraphicsComponent>()->getActiveSprite();

		sprite.setPosition(x, y);
		if (isFocused())
			sprite.setColor(sf::Color::White);
		else
			sprite.setColor(sf::Color(255, 255, 255, 32));

		window->draw(sprite);

		if(_highlightedItem == itemCount)
		{
			auto highlight = sf::RectangleShape(
				sf::Vector2f(padding + iconWidth, padding + iconHeight));
			highlight.setPosition(x - padding / 2, y - padding / 2);
			highlight.setFillColor(sf::Color::Transparent);
			highlight.setOutlineColor(sf::Color::Yellow);
			highlight.setOutlineThickness(2);
			window->draw(highlight);
		}

		itemCount++;
		if (itemCount == maxItems)
			break;
		if(itemCount % iconsPerRow == 0)
		{
			x = rectPos.x + padding;
			y = rectPos.y + padding + iconHeight + padding;
		} 
		else
		{
			x += iconWidth + padding;
		}
	}
}

sf::FloatRect Inventory::getBounds() 
{
	return _background.getGlobalBounds();
}
