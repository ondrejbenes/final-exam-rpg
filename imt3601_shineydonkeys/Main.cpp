// Hello :)
// Espen was here alot.
// THIS IS THE THING I CALL A MARK - G.d





#include <iostream>
#include <vector>
#include <stdlib.h> 
#include <math.h>
#include <random>
#include <algorithm>
#include <iterator>

#include <functional>

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


using namespace std;


int main(int argc, char* argv[])
{
	//copied from http://stackoverflow.com/questions/21516575/fill-a-vector-with-random-numbers-c
	// First create an instance of an engine.
	random_device rnd_device;
	// Specify the engine and distribution.
	mt19937 mersenne_engine(rnd_device());
	mersenne_engine.seed(1);
	uniform_int_distribution<int> dist(0, 6);

	auto gen = std::bind(dist, mersenne_engine);
	std::vector<int> vec(44*87 );
	generate(begin(vec), end(vec), gen);

	// Optional
	int x = 0;
	for (auto i : vec) {
		vec[x] = 3*(x<=88)+i*!(x <= 88);
		if ((x % 44 < 1) || (x>44 * 85)) { vec[x] = 3; };
	
		//if ((vec[x + 1] == 3) + (vec[x - 1] == 3) + (vec[x + 44] == 3) + (vec[x + 43] == 3)+(vec[x - 44] == 3) + (vec[x - 43] == 3)+ (vec[x - 88] == 3)+ (vec[x + 44] == 3) > 4) { vec[x] = 3; };
		std::cout << i << " ";
		x++;
	}




	int framecount = 0;
	int fps = 50;
	int animslowdown = 5;
	enum Direction { Down, Left, Right, Up, Still, Jump, DL,UL,UR,DR};
	sf::Vector2i source(0, Down);

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Shiny donkeys!");
	sf::View view;

	// Initialize the view to a rectangle located at (100, 100) and with a size of 400x200
	view.reset(sf::FloatRect(0, 0, 1280,720));
	// Rotate it by 45 degrees

	// Set its target viewport to be half of the window
	view.setViewport(sf::FloatRect(0.f, 0.f, 0.5f, 1.f));
	// Apply it
	window.setView(view);

	sf::View view2;

	// Initialize the view to a rectangle located at (100, 100) and with a size of 400x200
	view2.reset(sf::FloatRect(0, 0, 1280, 720));
	// Rotate it by 45 degrees

	// Set its target viewport to be half of the window
	view2.setViewport(sf::FloatRect(0.f, 0.f, 1, 1.f));
	// Apply it
	window.setView(view2);

	sf::RenderTexture texture;
	if (!texture.create(1280*1.1, 720*1.1))
		return -1;

	sf::Texture playerTexture;
	sf::Texture playerTexture2;
	sf::Sprite playerImage;

	sf::Texture TileTexture;
	sf::Sprite TileImage;

	sf::Texture bkgTexture;
	sf::Sprite bkgImage;

	

	sf::Clock clock;

	sf::SoundBuffer bkgmusicbuffer;
	sf::Sound bkgmusic;

	if (!bkgmusicbuffer.loadFromFile("Resources/Images/marchingbyriver_theme.ogg"))
		std::cout << "can't find sound file" << std::endl;

	bkgmusic.setBuffer(bkgmusicbuffer);
	

	bool updateFrame = true;

	if (!bkgTexture.loadFromFile("Resources/Images/profantasy-island.png"))
		std::cout << "Error: could not load bkg image" << std::endl;

	bkgImage.setTexture(bkgTexture);

	if (!playerTexture.loadFromFile("Resources/Images/4X8girl (6).png"))
		std::cout << "Error: could not load player image" << std::endl;

	if (!playerTexture2.loadFromFile("Resources/Images/girljump16x4.png"))
		std::cout << "Error: could not load player image" << std::endl;

	playerImage.setTexture(playerTexture);

	if (!TileTexture.loadFromFile("Resources/Images/tilex7.png"))
		std::cout << "Error: could not load tile image" << std::endl;
	TileImage.setTexture(TileTexture);

	while (window.isOpen())
	{
		framecount++;
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
		float speed = 100/16;
		playerImage.setTexture(playerTexture);

		/////////////////////////////

		/////////////////////////////////////////////////////GETTING KEYBOARD INPUT


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
		{

			bkgmusic.play();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
		{
		cout<<	fps++;

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
		{

			cout <<fps--;

		}

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
			source.x+=(0==framecount%animslowdown);//slow down animation
			source.x %= 8;
		}



		////////////////////////////////DRAWING SCREEN



		
		int quake = true;
		int x = 0;
		for (auto i : vec)
		{
			x++; //counts tiles
			TileImage.setTextureRect(sf::IntRect(i * 120, 0* 120, 120, 120));
			window.draw(TileImage);
			TileImage.setPosition(x%44*100+x/44%2*50,x/44*30-((framecount<180)*quake*(i*(180-framecount%180)))); //draws all tiles	
		}
		TileImage.setPosition(0, 0);

		//playerImage.setTextureRect(sf::IntRect(0,0, 120, 120));
		playerImage.setTextureRect(sf::IntRect(source.x * 120, source.y * 120, 120, 120));
		
		window.draw(playerImage);
	


		// create an array of 3 vertices that define a triangle primitive
		sf::VertexArray triangle(sf::Triangles, 3);

		// define the position of the triangle's points
		triangle[0].position = sf::Vector2f(-100*sin(framecount*.05), 10);
		triangle[1].position = sf::Vector2f(000, 10);
		triangle[2].position = sf::Vector2f(000, -300);

		// define the color of the triangle's points
		triangle[0].color = sf::Color::Red;
		triangle[1].color = sf::Color::Blue;
		triangle[2].color = sf::Color::Green;

		// no texture coordinates here, we'll see that later

		window.draw(triangle);

		// define the position of the triangle's points
		triangle[0].position = sf::Vector2f(200+100 * sin(framecount*.05), 10);
		triangle[1].position = sf::Vector2f(200, 10);
		triangle[2].position = sf::Vector2f(200, -300);

		// define the color of the triangle's points
		triangle[0].color = sf::Color::Red;
		triangle[1].color = sf::Color::Blue;
		triangle[2].color = sf::Color::Green;

		// no texture coordinates here, we'll see that later

		window.draw(triangle);



		sf::Vector2f position = playerImage.getPosition(); // = (15, 55)

		//sf::Sprite sprite(texture.getTexture());
		//window.draw(sprite);

		// Initialize the view to a rectangle located at (100, 100) and with a size of 400x200
		view2.reset(sf::FloatRect(position.x- 980 /2, position.y- 660 /2, 1280, 720));
		// Rotate it by 45 degrees
		view.reset(sf::FloatRect(0, 0, 1280*4, 720*8));
		// Set its target viewport to be half of the window
		view2.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

		view.setViewport(sf::FloatRect(0.025f, 0.5f, .275f, 1.f));
		// Apply it
		
	
		
		if (framecount>1000)window.setView(view);// minimap <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<-----------------
		else window.setView(view2);
		window.display();
	
		window.clear();


	

		sf::sleep(sf::milliseconds(1000/fps));//NTFS 60 FPS
	}

	return 0;
}