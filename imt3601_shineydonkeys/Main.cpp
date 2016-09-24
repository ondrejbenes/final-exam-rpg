
/*
Tiles 

1.Water			- blocks movement, but you can jump in it
2.Wet stone		- Slippery
3.Dry stone
4.Sand
5.Forest
6.Thick Forest	- You can hide, but only move back where you came from
7.Mud			- You will get stuck her 
8.Blocker		- You cant go here for some reason

Theese are the basic tiles, they can be replaced with overlays that look different but have same function as underlaying tiles.
*/

//#include <windows.h>
//#include "multimon.h"    
#include <string> 
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h> 
#include <math.h>
#include <random>
#include <algorithm>
#include <iterator>
#include <functional>
#include <SFML/Window/Joystick.hpp>
#include <SFML\Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Mouse.hpp>


using namespace std;






int main(int argc, char* argv[])
{












	//copied from http://stackoverflow.com/questions/21516575/fill-a-vector-with-random-numbers-c
	// First create an instance of an engine.
	random_device rnd_device;
	// Specify the engine and distribution.
	mt19937 mersenne_engine(rnd_device());
	mersenne_engine.seed(1);
	uniform_int_distribution<int> dist(12, 76);

	auto gen = std::bind(dist, mersenne_engine);
	std::vector<int> vec(44 * 87);
	generate(begin(vec), end(vec), gen);

	int x = 0;
	ofstream myfile;
	bool savelevel = false;
		if (savelevel)

		{
		
	
	myfile.open("level02.txt");
	for (auto i : vec) {

		//if ((x % 44 < 3) || (x<44 * 3) || (x>44 * 41)) { vec[x] = 0; };//add border of water
		myfile << i << "  ";
		myfile << endl;
		x++;
	}
	myfile.close();
}


	

	bool loadlevelfromfile = true;
		if (loadlevelfromfile)
		{
			std::ifstream input("level02.txt");
			x = 0;
			for (std::string line; getline(input, line); )
			{
				vec[x++] = stoi(line);
				if ((x % 44 < 3) || (x<44 * 3) || (x>44 * 41)) { vec[x] = 0; };//add border of water
			}
			cout << x << " map ints loaded";
			input.close();
		}


	int framecount = 0;
	int getstatus = 99;
	int fps = 60;
	int animslowdown = 8;//to 60/8 fps
	
	enum Direction { Down, DL, Left, UL, Up,UR,Right, RL, Still, Jump};
	sf::Vector2i source(0, Down);
	source.y = Still;
	


	auto style = sf::Style::Fullscreen;
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Shiny donkeys!", style);
	window.setVerticalSyncEnabled(true); // call it once, after creating the window
	// as specified in http://www.sfml-dev.org/tutorials/2.0/window-window.php
	
	


	//sf::RenderTexture rwindow;

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

	
	//if (!rwindow.create(1280, 720))
		//return -1;

	sf::Texture playerTexture;
	sf::Texture playerTexture2;
	sf::Texture playerTexture3;

	sf::Sprite playerImage;
	sf::Sprite weaponImage;

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

	if (!playerTexture.loadFromFile("Resources/Images/noaxereally.png"))
		std::cout << "Error: could not load player image" << std::endl;

	if (!playerTexture3.loadFromFile("Resources/Images/axe_masked.png"))
		std::cout << "Error: could not load player image" << std::endl;

	if (!playerTexture2.loadFromFile("Resources/Images/bruteaxe.png"))
		std::cout << "Error: could not load player image" << std::endl;

	playerImage.setTexture(playerTexture);

	if (!TileTexture.loadFromFile("Resources/Images/squaretiles.png"))
		std::cout << "Error: could not load tile image" << std::endl;
	TileImage.setTexture(TileTexture);

	// Declare a new font
	sf::Font font;
	// Load it from a file
	if (!font.loadFromFile("Resources/Fonts/sans.ttf"))
	{
		// error...
	}
	// Create a text which uses our font
	sf::Text text1,text2;


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
		float speed = 100/10;
		playerImage.setTexture(playerTexture);
		weaponImage.setTexture(playerTexture3);

		/////////////////////////////

		/////////////////////////////////////////////////////GETTING KEYBOARD INPUT
		bool testingjoystick = false;
		if (testingjoystick)
		{
			// Is joystick #0 connected?
			bool jconnected = sf::Joystick::isConnected(0);
			// How many buttons does joystick #0 support?
			unsigned int buttons = sf::Joystick::getButtonCount(0);
			// Does joystick #0 define a X axis?
			bool jhasX = sf::Joystick::hasAxis(0, sf::Joystick::X);
			// Is button #2 pressed on joystick #0?
			bool jpressed = sf::Joystick::isButtonPressed(0, 2);
			// What's the current position of the Y axis on joystick #0?
			float jpositiony = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
			float jpositionx = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
			float jpositiony1 = sf::Joystick::getAxisPosition(1, sf::Joystick::Y);
			float jpositionx1 = sf::Joystick::getAxisPosition(1, sf::Joystick::X);

			if (0 == framecount % 10)
				if (jconnected)
				{
					cout << "joystick has" << buttons << "jpressed" << jpressed << "jpx" << jpositiony << "jpy" << jpositionx;
					cout << "\n";
					cout << "joystick has" << buttons << "jpressed" << jpressed << "jpx" << jpositiony1 << "jpy" << jpositionx1;
					cout << "\n";
				}
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			int x = 0;
			myfile.open("level02.txt");
			for (auto i : vec) {

				//if ((x % 44 < 3) || (x<44 * 3) || (x>44 * 41)) { vec[x] = 0; };//add border of water
				myfile << i << "  ";
				myfile << endl;
				x++;
			}
			myfile.close();
			cout << "File saved";
		} 

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{

			window.create(sf::VideoMode(1280, 720), "Shiny donkeys!");
			window.setVerticalSyncEnabled(true); // call it once, after creating the window
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		{

			auto style = sf::Style::Fullscreen;
			window.create(sf::VideoMode(1280, 720), "Shiny donkeys!", style);
			window.setVerticalSyncEnabled(true); // call it once, after creating the window
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
		{

			auto style = sf::Style::None;
			window.create(sf::VideoMode(sf::VideoMode::getDesktopMode()), "Shiny donkeys!", style);
			window.setVerticalSyncEnabled(true); // call it once, after creating the window
			
		}
		

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

		
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) <- 50)
		{
			source.x *= (source.y == Up);//zero if new direction
			source.y = Up;
		
		}
	
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) >50)
		{
		source.x*= (source.y == Down) ;//zero if new direction
		source.y = Down;
	
		
		}
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 50)
		{
			source.x *= (source.y == Right);
		source.y = Right;
		}
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||  sf::Joystick::getAxisPosition(0, sf::Joystick::X) <-50)
		{
			source.x *= (source.y == Left);
			source.y = Left;
		}
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) || sf::Joystick::isButtonPressed(0, 2))
		{
			source.x *= (source.y == Jump);
 			source.y = Jump;
		}
		else {
			keyPressed = false;
			sf::Vector2f position = playerImage.getPosition();
			if (getstatus+1==(int(position.x)%80==20)+ (int(position.y) % 50 == 30))
			{
				source.y = Still;
				playerImage.move(0, 0);
				vec[(position.x - 20) / 80 + 1 + ((position.y - 30) / 50 + 3) * 44+1] = 0;
				cout << endl<<"x:"<<(position.x-20)/80+1<<"y:"<<(position.y-30)/50+3<<"tile:"<<(position.x - 20) / 80 + 1+ ((position.y - 30) / 50 + 3)*44;
			}
			getstatus = ((int(position.x) % 80 == 20) + (int(position.y) % 50 == 30));
		}

		if (source.y == Jump)
		{
			playerImage.move(0 * speed, 0);
			playerImage.setTexture(playerTexture2);
		}

		if (source.y == Left)
		{
			playerImage.move(-1 * speed, 0);
		
		}

		if (source.y == Right)
		{
		
			playerImage.move(1 * speed, 0);

		}

		if (source.y == Down)
		{
			playerImage.move(0, 1 * speed);

		}


		if (source.y == Up)
		{
			playerImage.move(0, -1 * speed);

		}


		{
			source.x+=(0==framecount%animslowdown);//slow down animation
			source.x %= 64;
		}



		////////////////////////////////DRAWING SCREEN



		
		int quake = true;
		int x = 0;
		int tile_x = 80;
		int tile_y = 50;
		int rows_x = 44;
		int rows_y = 44;;

		
		text1.setFont(font);
		text1.setCharacterSize(14);
		text1.setStyle(sf::Text::Regular);
		
		window.draw(text1);
		for (auto i : vec)
		{
			

			TileImage.setTextureRect(sf::IntRect(i%7 * 120, 0* 120, 120, 120));
			window.draw(TileImage);
			//TileImage.setPosition(x%44*100+x/44%2*50,x/44*30-((framecount<180)*quake*(i%7*(180-framecount%180)))); //draws all tiles	
			TileImage.setPosition(x % rows_x * tile_x, x / rows_y * tile_y); //draws all tiles	
			x++; //counts tiles
			
		}
		
		
		playerImage.setTextureRect(sf::IntRect(source.x%8 * 240, source.y%8 * 240, 240, 240));
		weaponImage.setTextureRect(sf::IntRect(source.x % 8 * 240, source.y % 8 * 240, 240, 240));
		weaponImage.setPosition( playerImage.getPosition());
		window.draw(playerImage);
		if (framecount%100<50) window.draw(weaponImage);
	


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
		
	
		
		//window.setView(view);// minimap <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<-----------------


		

		window.setView(view2);

		window.display();
		window.clear();
		
		



	

		sf::sleep(sf::milliseconds(1000/fps));//NTFS 60 FPS
	}


	return 0;


}
