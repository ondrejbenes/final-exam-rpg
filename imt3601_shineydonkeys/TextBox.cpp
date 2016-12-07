#include "TextBox.h"

TextBox::TextBox(const sf::Font& font, const std::string& text, unsigned fontSize, const sf::Color& textColor) 
{
	_drawableText = sf::Text(text, font, fontSize);

	resetOutlineSize();
	_outline.setFillColor(sf::Color::Transparent);
	_outline.setOutlineColor(sf::Color(255, 255, 255, 64));
	_outline.setOutlineThickness(1.5);

	setOnTextEntered(
		new UiCallback(
		[](UiElement* source, const sf::Event& event)
		{
			auto sourceAsTextBox = dynamic_cast<TextBox*>(source);
			auto str = sourceAsTextBox->getText();

			if (event.text.unicode == 8)
				str = str.substr(0, str.length() - 1);
			else
				str += event.text.unicode;

			sourceAsTextBox->setText(str);
		})
	);
}

void TextBox::draw(std::shared_ptr<sf::RenderWindow> window) 
{
	window->draw(_outline);
	
	if (!_focused)
		return;

	if (_verticalLineBlinkTimer.getElapsedTime() >= blinkFreq)
	{
		_verticalLineBlinkTimer.restart();
		if (showingVerticalLine)
		{
			std::string currStr = _drawableText.getString();
			currStr.replace(currStr.find('|'), 1, "");
			_drawableText.setString(currStr);
		}
		else
		{
			_drawableText.setString(_drawableText.getString() + '|');
		}
		showingVerticalLine = !showingVerticalLine;
	}

	window->draw(_drawableText);
}

sf::FloatRect TextBox::getBounds() 
{
	return _outline.getGlobalBounds();
}

void TextBox::setPosition(const sf::Vector2f& pos) 
{
	UiElement::setPosition(pos);
	_outline.setPosition(pos);
	_drawableText.setPosition(pos + sf::Vector2f(padding, padding / 2));
}

void TextBox::setFocused(bool focused) 
{
	if(focused == false && showingVerticalLine)
	{
		std::string str = _drawableText.getString();
		str.replace(str.find('|'), 1, "");
		_drawableText.setString(str);
		showingVerticalLine = false;
	}

	UiElement::setFocused(focused);
}

std::string TextBox::getText() 
{
	std::string str = _drawableText.getString();
	
	if(showingVerticalLine)
		str.replace(str.find('|'), 1, "");
	
	return str;
}

void TextBox::setText(const std::string& text) 
{
	if (showingVerticalLine)
		_drawableText.setString(text + '|');
	else
		_drawableText.setString(text);

	resetOutlineSize();
}

void TextBox::resetOutlineSize()
{
	auto width = std::max(50.0f, _drawableText.getGlobalBounds().width + 2 * padding);
	auto height = _drawableText.getCharacterSize() + 2 * padding;
	_outline.setSize(sf::Vector2f(width, height));
}

const sf::Time TextBox::blinkFreq = sf::milliseconds(500);
