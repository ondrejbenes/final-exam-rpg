#include "Menu.h"
#include <SFML/Graphics/Text.hpp>

Menu::Menu()
{

}

Menu::~Menu()
{

}

void Menu::update()
{

}

void Menu::render(sf::RenderWindow* window)
{	
	auto uiElements = _ui.getElements();
	for (auto it = uiElements.begin(); it != uiElements.end(); ++it)
		window->draw(**it);
}
