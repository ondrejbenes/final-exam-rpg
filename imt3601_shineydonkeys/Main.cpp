// Hello :)
// Espen was here alot.


#include <SFML/Graphics.hpp>
//#include <SMFL/Audio.hpp>


#include <iostream>

int main(int argc, char* argv[])
{
	enum Direction { Down, Left, Right, Up, Still, Jump, DL,UL,UR,DR};
	sf::Vector2i source(0, Down);

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Shiny donkeys!");

	sf::Texture playerTexture;
	sf::Texture playerTexture2;
	sf::Sprite playerImage;

	sf::Texture bkgTexture;
	sf::Sprite bkgImage;

	sf::Clock clock;

	//sf::Soundbuffer bkgmusicbuffer;
	//sf::Sound bkgmusic;

	//if (!soundBuffer.LoadFromFile("Resources/Images/marchingbyriver_theme.ogg"))
		//std::cout << "can't find sound file" << std::endl;

	//sound.setBuffer(soundBuffer);
	//sound.play();

	bool updateFrame = true;

	if (!bkgTexture.loadFromFile("Resources/Images/profantasy-island.png"))
		std::cout << "Error: could not load bkg image" << std::endl;

	bkgImage.setTexture(bkgTexture);

	if (!playerTexture.loadFromFile("Resources/Images/4X8girl (5).png"))
		std::cout << "Error: could not load player image" << std::endl;

	if (!playerTexture2.loadFromFile("Resources/Images/girljump16x4.png"))
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
			playerImage.setTexture(playerTexture);
			source.y = Up;
			playerImage.move(0, -1 * speed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			playerImage.setTexture(playerTexture);
			source.y = Down;
			playerImage.move(0, 1 * speed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			playerImage.setTexture(playerTexture);
			source.y = Right;
			playerImage.move(1 * speed, 0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			playerImage.setTexture(playerTexture);
			source.y = Left;
			playerImage.move(-1 * speed, 0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			playerImage.setTexture(playerTexture);
			source.y = Jump;
			playerImage.move(0 * speed, 0);
			playerImage.setTexture(playerTexture2);
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


		playerImage.setTextureRect(sf::IntRect(0,0, 239, 239));
		window.draw(bkgImage);

		playerImage.setTextureRect(sf::IntRect(source.x * 240, source.y *240, 240, 240));

		window.draw(playerImage);
		window.display();
		window.clear();

		sf::sleep(sf::milliseconds(1000/30));//NTFS 30 FPS
	}

	return 0;
}