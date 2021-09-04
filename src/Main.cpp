#include "Platform/Platform.hpp"
#include <lib.hpp>
#include <vector>
#include <pthread.h>

void *mythread(void *data);

using namespace std;
using namespace sf;

// float getYval(float x);
// float getYval(float x)
// {
// 	float result = currentPoints[0][1] * ((x - currentPoints[1][0]) / (currentPoints[0][0] - currentPoints[1][0]) * (x - currentPoints[2][0]) / (currentPoints[0][0] - currentPoints[2][0]));
// 	result += currentPoints[1][1] * ((x - currentPoints[0][0]) / (currentPoints[1][0] - currentPoints[0][0]) * (x - currentPoints[2][0]) / (currentPoints[1][0] - currentPoints[2][0]));
// 	result += currentPoints[2][1] * ((x - currentPoints[0][0]) / (currentPoints[2][0] - currentPoints[0][0]) * (x - currentPoints[1][0]) / (currentPoints[2][0] - currentPoints[1][0]));
// 	return result;
// }

int main()
{
	util::Platform platform;

#if defined(_DEBUG)
	std::cout << "Hello World!" << std::endl;
#endif
	float quality;
	vector<parabola> parabolas;
	float offset[2] = { 500, 500 };
	bool mouseBtn[2] = { 0, 0 };
	float newPoints[3][2];
	bool draw = false;
	int mouseCord[2];
	int oldMouseCord[2] = { 0, 0 };
	int currentPointNum = 0;
	quality = 100;

	sf::RenderWindow window;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	window.create(sf::VideoMode(1000.0f * screenScalingFactor, 1000.0f * screenScalingFactor), "SFML Grapher!");
	platform.setIcon(window.getSystemHandle());

	// Limit the framerate to 60 frames per second (this step is optional)
	window.setFramerateLimit(60);

	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(150, 150))
	};

	sf::CircleShape pointDraw(3);
	pointDraw.setFillColor(sf::Color::Red);

	sf::CircleShape pointDrawNew(3);
	pointDrawNew.setFillColor(sf::Color::Blue);

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			// catch the resize events
			if (event.type == sf::Event::Resized)
			{
				// update the view to the new size of the window
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}

			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.delta < 0)
				{
					quality += 1;
				}
				else
				{
					if (quality > 1)
						quality -= 1;
				}
			}
		}

		mouseBtn[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		mouseBtn[1] = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		mouseCord[0] = sf::Mouse::getPosition(window).x;
		mouseCord[1] = sf::Mouse::getPosition(window).y;

		if (mouseBtn[0])
		{
			offset[0] += mouseCord[0] - oldMouseCord[0];
			offset[1] += mouseCord[1] - oldMouseCord[1];
			// cout << offset[0] << "|" << offset[1] << endl;
		}

		if (mouseBtn[1])
		{
			cout << mouseCord[0] - window.getSize().x / (2 * quality) + offset[0] << endl;
			newPoints[currentPointNum][0] = mouseCord[0] * quality - window.getSize().x / 2 - (offset[0] * quality);
			newPoints[currentPointNum][1] = mouseCord[1] * quality - window.getSize().y / 2 - (offset[1] * quality);

			if (currentPointNum == 2)
			{
				draw = true;
				parabola newParabola;
				for (int i = 0; i < 3; i++)
				{
					newParabola.currentPoints[i][0] = newPoints[i][0];
					newParabola.currentPoints[i][1] = newPoints[i][1];
				}
				parabolas.push_back(newParabola);
				currentPointNum = 0;
			}
			else
			{
				currentPointNum++;
			}

			while (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{}
		}

		oldMouseCord[0] = mouseCord[0];
		oldMouseCord[1] = mouseCord[1];

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (quality > 1)
			{
				quality--;
			}
			while (Keyboard::isKeyPressed(Keyboard::Up))
			{}
		}

		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			quality++;
			while (Keyboard::isKeyPressed(Keyboard::Down))
			{}
		}

		window.clear();

		line[0].color = Color::Red;
		line[1].color = Color::Red;

		line[0].position = sf::Vector2f(window.getSize().x / (2 * quality) + offset[0], 0);
		line[1].position = sf::Vector2f(window.getSize().x / (2 * quality) + offset[0], window.getSize().y);
		window.draw(line, 2, sf::Lines);

		line[0].color = sf::Color::Green;
		line[1].color = sf::Color::Green;

		line[0].position = sf::Vector2f(0, window.getSize().y / (2 * quality) + offset[1]);
		line[1].position = sf::Vector2f(window.getSize().x, window.getSize().y / (2 * quality) + offset[1]);
		window.draw(line, 2, sf::Lines);

		line[0].color = sf::Color::White;
		line[1].color = sf::Color::White;
		for (int i = 0; i < 3; i++)
		{
			pointDrawNew.setPosition(sf::Vector2f(((newPoints[i][0] / quality) + offset[0]) - 3 + window.getSize().x / (2 * quality), ((newPoints[i][1] / quality) + offset[1]) - 3 + window.getSize().y / (2 * quality)));
			window.draw(pointDrawNew);
		}
		if (draw)
		{
			for (auto currentParabola : parabolas)
			{
				line[0].color = sf::Color(currentParabola.colour[0], currentParabola.colour[1], currentParabola.colour[2]);
				line[1].color = sf::Color(currentParabola.colour[0], currentParabola.colour[1], currentParabola.colour[2]);
				for (int x = 0 - (offset[0] * quality); x <= (int(window.getSize().x) * quality) - offset[0]; x += quality)
				{
					float x1 = x - int(window.getSize().x / 2);
					float y = currentParabola.getYval(x1) + int(window.getSize().y) / 2;
					if (x == 0 - (offset[0] * quality))
					{
						line[1].position = sf::Vector2f((x / quality) + offset[0], (y / quality) + offset[1]);
					}
					else
					{
						line[0] = line[1];
						line[1].position = sf::Vector2f((x / quality) + offset[0], (y / quality) + offset[1]);
						window.draw(line, 2, sf::Lines);
					}
				}
				for (int i = 0; i < 3; i++)
				{
					pointDraw.setPosition(sf::Vector2f(((currentParabola.currentPoints[i][0] / quality) + offset[0]) - 3 + window.getSize().x / (2 * quality), ((currentParabola.currentPoints[i][1] / quality) + offset[1]) - 3 + window.getSize().y / (2 * quality)));
					pointDraw.setFillColor(sf::Color(currentParabola.colourPoint[0], currentParabola.colourPoint[1], currentParabola.colourPoint[2]));
					window.draw(pointDraw);
				}
			}
		}
		window.display();
	}

	return 0;
}
