// Hello :)
// Espen was here alot.
// THIS IS THE THING I CALL A MARK - G.d





#include <iostream>
//#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio/InputSoundFile.hpp>
#include <SFML/Audio/Listener.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/OutputSoundFile.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/SoundBufferRecorder.hpp>
#include <SFML/Audio/SoundFileFactory.hpp>
#include <SFML/Audio/SoundFileReader.hpp>
#include <SFML/Audio/SoundFileWriter.hpp>
#include <SFML/Audio/SoundRecorder.hpp>
#include <SFML/Audio/SoundSource.hpp>
#include <SFML/Audio/SoundStream.hpp>

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

	if (!playerTexture.loadFromFile("Resources/Images/4X8girl (6).png"))
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
		playerImage.setTexture(playerTexture);
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
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
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
			source.x %= 8;
		}


		playerImage.setTextureRect(sf::IntRect(0,0, 120, 120));
		window.draw(bkgImage);

		playerImage.setTextureRect(sf::IntRect(source.x * 120, source.y *120, 120, 120));

		window.draw(playerImage);
	
		window.display();
	
		window.clear();

	

		sf::sleep(sf::milliseconds(1000/30));//NTFS 30 FPS
	}

	return 0;
}