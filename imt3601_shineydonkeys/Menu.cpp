#include "Menu.h"
#include "Blackboard.h"

void Menu::update()
{
	handleInput();
}

void Menu::handleInput()
{
	auto blackboard = Blackboard::getInstance();
	sf::Event event;
	while (blackboard->pollEvent(event))
		handleGuiEvent(event);
}