#include<vector>
#include<fstream>
#include<iostream>

#include"Song.h"

double SineWave(double time, double freq, double amplitude)
{
	short result;
	double tpc = 44100 / freq; // Ticks per cycle (samples per second)
	double cycles = time / tpc; // Total cycles
	double rad = cycles * 6.282; // radian to be sined
	double amp = amplitude * 32767; // amplitude
	result = amp * sin(rad);
	return result;
}

short AF(double time) {
	//                 c                            e                               g
	return SineWave(time, 261.63, 0.3) + SineWave(time, 329.63, 0.3) +  SineWave(time, 392, 0.3);
}

int main()
{
	std::ifstream file("hsct.txt");

	sf::RenderWindow window(sf::VideoMode(330, 330), "Our Program");
	Song song(file,100,1);

	song.transpose(2);

	sf::Sprite sprite;
	sf::Texture texture;
	if (!texture.loadFromFile("cmd.png")) {
		std::cout << "couldn't open the image\n";
	}
	sprite.setTexture(texture);
	

	while (window.isOpen())
	{
		window.draw(sprite);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				song.playSong();
			}
		}
		window.display();
	}

	return 0;
}