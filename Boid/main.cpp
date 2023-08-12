#include "Boid.h"
#include "Vector2D.h"
#include <iostream>

vector<Boid> g_vectorBoid;

int main()
{
	// rand set
	srand(time(NULL));
	// creation of screen
	int fps = 60;
	std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>();
	window->create(sf::VideoMode(1920, 1080), "Boid Running", sf::Style::Default);
	window->setFramerateLimit(fps);
	window->setVerticalSyncEnabled(true);

	// creation of ai
	g_vectorBoid.reserve(10);
	for (int i = 0; i < 15; ++i)
	{
		// HIVEMIND && SEEK
		g_vectorBoid.emplace_back(Vector2D(rand() % 1920, rand() % 1080), Vector2D::FRONT, 10,STATE::PATHFOLLOW, 45.0f, 300.0f, i);
		Boid& boid = g_vectorBoid[i];
		boid.SetWindow(window);
		boid.m_obstacleVec.emplace_back();
	}

	// loop
	sf::Clock clock;
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}
		float deltaTime = clock.restart().asSeconds(); // delta time
		window->clear(sf::Color::Black);
		for (int i = 0; i < g_vectorBoid.size(); ++i)
		{
			g_vectorBoid[i].Update(deltaTime, g_vectorBoid);
			g_vectorBoid[i].Render(window);
		}

		for (int j = 0; j < g_vectorBoid.size(); ++j)
		{
			for (int i = 0; i < g_vectorBoid[j].m_obstacleVec.size(); ++i)
			{
				g_vectorBoid[j].m_obstacleVec[i].Render(window);
			}
		}

		// test path follow points
		sf::CircleShape point1;
		point1.setPosition(sf::Vector2f(100, 100));
		point1.setRadius(2);
		point1.setFillColor(sf::Color::Red);
		window->draw(point1);

		sf::CircleShape point2;
		point2.setPosition(sf::Vector2f(500, 300));
		point2.setRadius(2);
		point2.setFillColor(sf::Color::Red);
		window->draw(point2);

		sf::CircleShape point3;
		point3.setPosition(sf::Vector2f(900, 100));
		point3.setRadius(2);
		point3.setFillColor(sf::Color::Red);
		window->draw(point3);

		sf::CircleShape point4;
		point4.setPosition(sf::Vector2f(1300, 300));
		point4.setRadius(2);
		point4.setFillColor(sf::Color::Red);
		window->draw(point4);

		window->display();
	}
}