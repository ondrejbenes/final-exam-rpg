#include "LevelLoader.h"
#include "tinyxml2.h"
#include "tinyxml2.h"
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace tinyxml2;

LevelLoader::LevelLoader()
{
	XMLDocument doc;
	auto gameData = doc.LoadFile("Resources/levels/EntitiesData.xml");
	XMLElement* titleElement = doc.FirstChildElement("Level")->FirstChildElement("Player")->FirstChildElement("HitPoints")/*->FirstChildElement()*/;
	const char* HP = titleElement->GetText();
	printf( " %s ", HP);

	auto* nextElement = titleElement->NextSiblingElement()->GetText();
	printf(" %s ", nextElement);


	XMLElement* titleElement2 = doc.FirstChildElement("Level")->FirstChildElement("NPCs")->FirstChildElement();









	/*auto* titleElement2 = doc.FirstChildElement("Level")->FirstChildElement("NPCs")->
		LastChildElement("NPC")->FirstChildElement("Sprite");

	auto* titleElement2Value = titleElement2->FirstAttribute()->Value();
	printf(" 2Name of play : %s", titleElement2Value);
*/
	/*auto* nextElement2 = titleElement2->NextSiblingElement();
	auto* titleElement3Value = nextElement2->FirstAttribute()->Value();
	printf(" 3Name of play : %s", titleElement3Value);*/

	


	auto a = 0;
	

}


