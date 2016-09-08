// Hello :)
// Espen was here but he didnt touch anything, YET.

#include <SFML/Graphics.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
	enum Direction { Down, Left, Right, Up };
	sf::Vector2i source(0, Down);

	sf::RenderWindow window(sf::VideoMode(400, 400), "SFML works!");

	sf::Texture playerTexture;
	sf::Sprite playerImage;

	sf::Clock clock;

	bool updateFrame = true;

	if (!playerTexture.loadFromFile("Resources/Images/Player.png"))
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
		float speed = 5;

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
		}

		if (keyPressed)
		{
			source.x++;
			if (source.x * 32 >= playerTexture.getSize().x)
				source.x = 0;
		}

		playerImage.setTextureRect(sf::IntRect(source.x * 32, source.y * 48, 32, 48));

		window.draw(playerImage);
		window.display();
		window.clear();

		sf::sleep(sf::milliseconds(75));
	}

	return 0;
}