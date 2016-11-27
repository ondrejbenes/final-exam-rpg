#include "UiElementFactory.h"
#include "ConfigIO.h"
#include "StringUtilities.h"
#include "ResourceLoader.h"

#include <codecvt>
#include <sstream>


Button* UiElementFactory::createButton(const wchar_t* configFile, const wchar_t* configSectionName, UiCallback onClick)
{
	auto text = ConfigIO::readString(configSectionName, L"text", L"???", configFile);
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	auto fontSize = ConfigIO::readInt(configSectionName, L"fontSize", 20, configFile);
	auto x = ConfigIO::readInt(configSectionName, L"x", 20, configFile);
	auto y = ConfigIO::readInt(configSectionName, L"y", 20, configFile);

	auto uiElement = new Button(text, font, fontSize);
	uiElement->setOnClick(new UiCallback(onClick));
	uiElement->setPosition(sf::Vector2f(x, y));

	return uiElement;
}

Label* UiElementFactory::createLabel(const wchar_t* configFile, const wchar_t* configSectionName)
{
	auto text = ConfigIO::readString(configSectionName, L"text", L"???", configFile);
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	auto fontSize = ConfigIO::readInt(configSectionName, L"fontSize", 20, configFile);
	auto x = ConfigIO::readInt(configSectionName, L"x", 20, configFile);
	auto y = ConfigIO::readInt(configSectionName, L"y", 20, configFile);

	auto uiElement = new Label(text, font, fontSize);
	uiElement->setPosition(sf::Vector2f(x, y));

	return uiElement;
}

TextBox* UiElementFactory::createTextBox(const wchar_t* configFile, const wchar_t* configSectionName)
{
	auto text = ConfigIO::readString(configSectionName, L"text", L"???", configFile);
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	auto fontSize = ConfigIO::readInt(configSectionName, L"fontSize", 20, configFile);
	auto x = ConfigIO::readInt(configSectionName, L"x", 20, configFile);
	auto y = ConfigIO::readInt(configSectionName, L"y", 20, configFile);

	auto uiElement = new TextBox(font, text, fontSize);
	uiElement->setPosition(sf::Vector2f(x, y));

	return uiElement;
}

UiCallback* UiElementFactory::createChangeKeyButton(const std::wstring& moveDirection)
{
	auto lambda = [moveDirection](UiElement* source, const sf::Event& event)
	{
		auto sfCode = event.key.code;
		ConfigIO::writeInt(L"controls", moveDirection.c_str(), sfCode);

		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

		auto humanReadable = new char[20];
		StringUtilities::SFKeyToString(sfCode, humanReadable);
		std::stringstream ss;
		ss << "Move " << converter.to_bytes(moveDirection) << ": " << humanReadable;
		delete humanReadable;

		auto btnText = ss.str();
		auto sourceAsBtn = dynamic_cast<Button*>(source);
		sourceAsBtn->setText(btnText);

		std::wstring btnTextAsWStr;
		btnTextAsWStr.assign(btnText.begin(), btnText.end());

		std::wstringstream wss;
		wss << L"move" << wchar_t(towupper(moveDirection[0])) << std::nouppercase << moveDirection.substr(1) << L"Key";

		ConfigIO::writeString(wss.str().c_str(), L"text", btnTextAsWStr.c_str(), L"./Config/options.ini");
	};

	return new UiCallback(lambda);
}