#include "ChatBoard.h"
#include "ResourceLoader.h"
#include <sstream>
#include "Blackboard.h"
#include "Network.h"

ChatBoard::ChatBoard()
{
	_background.setSize(sf::Vector2f(width, height));

	_input = "";
}

void ChatBoard::draw(std::shared_ptr<sf::RenderWindow> window) 
{
	auto view = window->getView();
	auto viewRight = view.getCenter().x + view.getSize().x / 2;
	auto viewBottom = view.getCenter().y + view.getSize().y / 2;
	
	auto rectX = viewRight - width;
	auto rectY = viewBottom - height;

	_background.setPosition(rectX, rectY);
	
	if(isFocused())
		_background.setFillColor(sf::Color::Black);
	else
		_background.setFillColor(sf::Color(0, 0, 0, 64));

	window->draw(_background);

	auto padding = 5;
	auto linesCount = std::min(floor((height - 2 * padding) / fontSize), double(_messages.size()));
	
	auto firstMsgIdx = std::max(int(_messages.size() - linesCount), 0);

	auto& font = ResourceLoader::getInstance()->getConsoleFont();
	for (auto i = 0; i < linesCount; i++)
	{
		auto text = sf::Text(_messages[firstMsgIdx + i], font, fontSize);
		text.setFillColor(sf::Color::White);
		text.setPosition(rectX + padding, rectY + padding + fontSize * i);
		window->draw(text);
	}


	if(_input != "" || isFocused())
	{
		auto text = sf::Text(inputMarker + _input, font, fontSize);
		text.setFillColor(sf::Color::Magenta);
		text.setPosition(rectX + padding, rectY + padding + fontSize * linesCount);
		window->draw(text);		
	}

}

sf::FloatRect ChatBoard::getBounds() 
{
	return _background.getGlobalBounds();
}

void ChatBoard::addMessage(const std::string& author, const std::string& message) 
{
	std::stringstream ss;
	ss << author << ": " << message;
	auto msg = std::string(ss.str());
	_messages.push_back(msg);
}