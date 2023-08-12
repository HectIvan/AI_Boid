#pragma once
#include <SFML/Graphics.hpp>
#include "Vector2D.h"
class Obstacle
{
public:
	Obstacle();
	~Obstacle();
	sf::CircleShape m_circle;
	std::shared_ptr<sf::RenderWindow> m_window;
	int m_radius;

public:
	void Start();
	void Render(std::shared_ptr<sf::RenderWindow> window);
	void SetWindow(std::shared_ptr<sf::RenderWindow> window);

	Vector2D GetPosition();
	int GetRadius();
	sf::CircleShape GetShape();
};

