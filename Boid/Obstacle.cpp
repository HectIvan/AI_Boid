#include "Obstacle.h"

Obstacle::Obstacle()
{
}

Obstacle::~Obstacle()
{
}

void Obstacle::Start()
{
	m_radius = 20.0f;
	m_circle.setRadius(m_radius);
	m_circle.setFillColor(sf::Color::Red);
	m_circle.setPosition(300, 700);
	m_circle.setOrigin(sf::Vector2f(m_radius * 0.5f, m_radius * 0.5f));
}

void Obstacle::Render(std::shared_ptr<sf::RenderWindow> window)
{
	window->draw(m_circle);
}

void Obstacle::SetWindow(std::shared_ptr<sf::RenderWindow> window)
{
	m_window = window;
}

Vector2D Obstacle::GetPosition()
{
	return Vector2D(m_circle.getPosition().x, m_circle.getPosition().y);
}

int Obstacle::GetRadius()
{
	return m_radius;
}

sf::CircleShape Obstacle::GetShape()
{
	return m_circle;
}
