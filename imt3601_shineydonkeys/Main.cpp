// Hello :)
// Espen was here but he didnt touch anything, YET.

#include <SFML/Graphics.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
	enum Direction { Down, Left, Right, Up, Still, DL,UL,UR,DR};
	sf::Vector2i source(0, Down);

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Shiny donkeys!");

	sf::Texture playerTexture;
	sf::Sprite playerImage;

	sf::Clock clock;

	bool updateFrame = true;

	if (!playerTexture.loadFromFile("Resources/Images/4X8girl (5).png"))
		std::cout << "Error: could not load player image" << std::endl;

	playerImage.setTexture(playerTexture);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}

		bool keyPressed = true;
		float speed = 20;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			source.y = Up;
			playerImage.move(0, -1 * speed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			source.y = Down;
			playerImage.move(0, 1 * speed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			source.y = Right;
			playerImage.move(1 * speed, 0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			source.y = Left;
			playerImage.move(-1 * speed, 0);
		}
		else {
			keyPressed = false;
			source.y = Still;
			playerImage.move(0, 0);
		}

		if (keyPressed)
		{
			source.x++;
			//if (source.x * 32 >= playerTexture.getSize().x)
			//if (source.x >8)
				//source.x = 0;
			source.x = source.x *(source.x < 8);
			//sets x to 0 if 8 or above :) because i generally prefer bool math over if statements.
		}

		playerImage.setTextureRect(sf::IntRect(source.x * 240, source.y *240, 240, 240));

		window.draw(playerImage);
		window.display();
		window.clear();

		sf::sleep(sf::milliseconds(75));
	}

	return 0;
}