#include "Menu.h"
#include "Blackboard.h"

Menu::Menu()
{

}

Menu::~Menu()
{

}

void Menu::update()
{
	handleInput();
}

void Menu::render(std::shared_ptr<sf::RenderWindow> window)
{	
	
	GamePhase::render(window);
}

// TODO rename to something like handleUiCallbacks and move to gamephase
void Menu::handleInput()
{
	auto blackboard = Blackboard::getInstance();
	sf::Event event;
	while (blackboard->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonReleased:
			handleMouseReleased(event);
			break;
		case sf::Event::TextEntered:
			handleTextEntered(event);
			break;
		case sf::Event::KeyPressed:
			handleKeyPressed(event);
			break;
		default:
			break;
		}
	}
}

void Menu::handleKeyPressed(const sf::Event& event)
{
	auto focusedElement = _ui.getFocusedElement();
	if (focusedElement == nullptr)
		return;

	auto callback = focusedElement->getOnKeyPressed();
	if (callback != nullptr)
		(*callback)(focusedElement, event);
}