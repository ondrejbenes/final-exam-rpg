#include "StringUtilities.h"

#include <string>
#include <sstream>
#include <vector>
#include <SFML/Window/Keyboard.hpp>

std::string StringUtilities::doubleToBinaryStr(double value)
{
	auto doubleAsString = std::to_string(value);
	auto beforeDecimalPointStr = doubleAsString.substr(doubleAsString.length() - doubleAsString.find('.'));

	std::string afterDecimalPointStr = "";

	double afterDecimalPoint = 2 * (abs(value) - abs(long(value)));
	for (int i = 0; i < 15; i++)
	{
		if (afterDecimalPoint >= 1)
		{
			afterDecimalPointStr += "1";
			if (abs(afterDecimalPoint - 1) < pow(10, -8)) 
				break;
			afterDecimalPoint -= 1;
		}
		else
		{
			afterDecimalPointStr += "0";
		}
		afterDecimalPoint *= 2;
	}

	return beforeDecimalPointStr + "." + afterDecimalPointStr;
}

std::vector<std::string> StringUtilities::split(const std::string &s, char delim) 
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

bool StringUtilities::startsWith(std::string& str, std::string& start)
{
	return str.compare(0, start.length(), start) == 0;
}

void StringUtilities::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}

void StringUtilities::SFKeyToString(unsigned int keycode, char *keyStr) {
	if(keycode <= 25)
	{
		char asci = keycode + 65;
		std::string str = "";
		str += asci;
		sprintf(keyStr, str.c_str());
		return;
	}

	switch (keycode) {
		case sf::Keyboard::Key::Escape: sprintf(keyStr, "Escape"); break;
		case sf::Keyboard::Key::LControl: sprintf(keyStr, "LControl"); break;
		case sf::Keyboard::Key::LShift: sprintf(keyStr, "LShift"); break;
		case sf::Keyboard::Key::LAlt: sprintf(keyStr, "LAlt"); break;
		case sf::Keyboard::Key::LSystem: sprintf(keyStr, "LSystem"); break;
		case sf::Keyboard::Key::RControl: sprintf(keyStr, "RControl"); break;
		case sf::Keyboard::Key::RShift: sprintf(keyStr, "RShift"); break;
		case sf::Keyboard::Key::RAlt: sprintf(keyStr, "RAlt"); break;
		case sf::Keyboard::Key::RSystem: sprintf(keyStr, "RSystem"); break;
		case sf::Keyboard::Key::Menu: sprintf(keyStr, "Menu"); break;
		case sf::Keyboard::Key::LBracket: sprintf(keyStr, "LBracket"); break;
		case sf::Keyboard::Key::RBracket: sprintf(keyStr, "RBracket"); break;
		case sf::Keyboard::Key::SemiColon: sprintf(keyStr, ";"); break;
		case sf::Keyboard::Key::Comma: sprintf(keyStr, ","); break;
		case sf::Keyboard::Key::Period: sprintf(keyStr, "."); break;
		case sf::Keyboard::Key::Quote: sprintf(keyStr, "\'"); break;
		case sf::Keyboard::Key::Slash: sprintf(keyStr, "/"); break;
		case sf::Keyboard::Key::BackSlash: sprintf(keyStr, "\\"); break;
		case sf::Keyboard::Key::Tilde: sprintf(keyStr, "~"); break;
		case sf::Keyboard::Key::Equal: sprintf(keyStr, "="); break;
		case sf::Keyboard::Key::Dash: sprintf(keyStr, "-"); break;
		case sf::Keyboard::Key::Space: sprintf(keyStr, "Space"); break;
		case sf::Keyboard::Key::Return: sprintf(keyStr, "Return"); break;
		case sf::Keyboard::Key::BackSpace: sprintf(keyStr, "BackSpace"); break;
		case sf::Keyboard::Key::Tab: sprintf(keyStr, "Tab"); break;
		case sf::Keyboard::Key::PageUp: sprintf(keyStr, "Page Up"); break;
		case sf::Keyboard::Key::PageDown: sprintf(keyStr, "Page Down"); break;
		case sf::Keyboard::Key::End: sprintf(keyStr, "End"); break;
		case sf::Keyboard::Key::Home: sprintf(keyStr, "Home"); break;
		case sf::Keyboard::Key::Insert: sprintf(keyStr, "Insert"); break;
		case sf::Keyboard::Key::Delete: sprintf(keyStr, "Delete"); break;
		case sf::Keyboard::Key::Add: sprintf(keyStr, "+"); break;
		case sf::Keyboard::Key::Subtract: sprintf(keyStr, "-"); break;
		case sf::Keyboard::Key::Multiply: sprintf(keyStr, "*"); break;
		case sf::Keyboard::Key::Divide: sprintf(keyStr, "/"); break;
		case sf::Keyboard::Key::Left: sprintf(keyStr, "Left"); break;
		case sf::Keyboard::Key::Right: sprintf(keyStr, "Right"); break;
		case sf::Keyboard::Key::Up: sprintf(keyStr, "UP"); break;
		case sf::Keyboard::Key::Down: sprintf(keyStr, "Down"); break;
		case sf::Keyboard::Key::Numpad0: sprintf(keyStr, "NP 0"); break;
		case sf::Keyboard::Key::Numpad1: sprintf(keyStr, "NP 1"); break;
		case sf::Keyboard::Key::Numpad2: sprintf(keyStr, "NP 2"); break;
		case sf::Keyboard::Key::Numpad3: sprintf(keyStr, "NP 3"); break;
		case sf::Keyboard::Key::Numpad4: sprintf(keyStr, "NP 4"); break;
		case sf::Keyboard::Key::Numpad5: sprintf(keyStr, "NP 5"); break;
		case sf::Keyboard::Key::Numpad6: sprintf(keyStr, "NP 6"); break;
		case sf::Keyboard::Key::Numpad7: sprintf(keyStr, "NP 7"); break;
		case sf::Keyboard::Key::Numpad8: sprintf(keyStr, "NP 8"); break;
		case sf::Keyboard::Key::Numpad9: sprintf(keyStr, "NP 9"); break;
		case sf::Keyboard::Key::F1: sprintf(keyStr, "F1"); break;
		case sf::Keyboard::Key::F2: sprintf(keyStr, "F2"); break;
		case sf::Keyboard::Key::F3: sprintf(keyStr, "F3"); break;
		case sf::Keyboard::Key::F4: sprintf(keyStr, "F4"); break;
		case sf::Keyboard::Key::F5: sprintf(keyStr, "F5"); break;
		case sf::Keyboard::Key::F6: sprintf(keyStr, "F6"); break;
		case sf::Keyboard::Key::F7: sprintf(keyStr, "F7"); break;
		case sf::Keyboard::Key::F8: sprintf(keyStr, "F8"); break;
		case sf::Keyboard::Key::F9: sprintf(keyStr, "F9"); break;
		case sf::Keyboard::Key::F10: sprintf(keyStr, "F10"); break;
		case sf::Keyboard::Key::F11: sprintf(keyStr, "F11"); break;
		case sf::Keyboard::Key::F12: sprintf(keyStr, "F12"); break;
		case sf::Keyboard::Key::F13: sprintf(keyStr, "F13"); break;
		case sf::Keyboard::Key::F14: sprintf(keyStr, "F14"); break;
		case sf::Keyboard::Key::F15: sprintf(keyStr, "F15"); break;
		case sf::Keyboard::Key::Pause: sprintf(keyStr, "Pause"); break;
	default:
		sprintf(keyStr, "%c", keycode);
	}
}