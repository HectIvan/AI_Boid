#include "Boid.h"
#include <iostream>
Boid::Boid()
{
}

Boid::Boid(Vector2D position, Vector2D direction, float radius, int currentState, float visualRange, float speed, int ID, float mass)
{
	m_position = position;
	m_direction = direction;
	m_currentState = currentState;

	m_circleShape.setRadius(radius);
	m_circleShape.setOrigin(radius * 0.5f, radius * 0.5f);
	m_circleShape.setPointCount(3);
	m_circleShape.setFillColor(sf::Color::White);
	m_timer = 0;
	m_ID = ID;
	m_visualCone = visualRange;
	m_speed = speed;
	m_mass = mass;

	m_pathPoints.resize(4);
	m_pathPoints[0] = Vector2D(100, 100);
	m_pathPoints[1] = Vector2D(500, 300);
	m_pathPoints[2] = Vector2D(900, 100);
	m_pathPoints[3] = Vector2D(1300, 300);
}
void DrawLines(Vector2D start, Vector2D end);
Boid::~Boid()
{
}
sf::VertexArray g_lines(sf::LinesStrip, 2);
void Boid::Update(float DeltaTime, vector<Boid>& vecBoid)
{
	float perc = 1;
	const shared_ptr<sf::RenderWindow> CWindow = m_window;

	
	if (m_checkCurs == 0) { m_prevCursPos = Vector2D(sf::Mouse::getPosition(*CWindow.get()).x, sf::Mouse::getPosition(*CWindow.get()).y); ++m_checkCurs;  }
	else { m_checkCurs = 0; }

	Vector2D cursorPos(sf::Mouse::getPosition(*CWindow.get()).x, sf::Mouse::getPosition(*CWindow.get()).y);

	Vector2D desiredForce = Vector2D(0.0f);

	if ((m_currentState & STATE::SEEK) == STATE::SEEK)
	{
		desiredForce += Seek(cursorPos, 8.0f);
	}
	if ((m_currentState & STATE::FLEE) == STATE::FLEE)
	{
		//m_circleShape.setFillColor(sf::Color::White);
		desiredForce += Flee(cursorPos, 100, 6.0f);
	}
	if ((m_currentState & STATE::PURSUIT) == STATE::PURSUIT)
	{
		//cursorPos - m_prevCursPos
		desiredForce += Pursuit(cursorPos, m_prevCursPos, 10, true);
	}
	if ((m_currentState & STATE::EVADE) == STATE::EVADE) 
	{
		desiredForce += Evade(cursorPos, m_prevCursPos, 100, 10, true);
	}
	if ((m_currentState & STATE::PATHFOLLOW) == STATE::PATHFOLLOW)
	{
		desiredForce += PathFollowing(m_pathPoints, 6.0f);
		if ((m_pathPoints[m_i] - m_position).Magnitude() < 200)
		{
			++m_i;
		}
	}
	if ((m_currentState & STATE::WANDER) == STATE::WANDER)
	{
		m_timer += DeltaTime;
		Vector2D pos = Vector2D(0);
		pos = Wander(100, m_visualCone);
		desiredForce += Seek(pos, 5);
		Vector2D a = m_direction * m_speed + desiredForce;
		m_direction = a;
		m_direction.Normalize();
		m_position += a * DeltaTime;
	}
	if ((m_currentState & STATE::QUEUE) == STATE::QUEUE) 
	{
		if (m_ID != vecBoid.size() - 1)
		{
			desiredForce += Queue(vecBoid, 20, 5, perc);
		}
		else
		{
			m_currentState = STATE::ARRIVAL;
			m_circleShape.setFillColor(sf::Color::Red);
		}
	}
	if ((m_currentState & STATE::LEADERFOLLOW) == STATE::LEADERFOLLOW) 
	{
		if (m_ID != vecBoid.size() - 1)
		{
			desiredForce += LeaderFollowing(vecBoid, 20, 5, perc);
		}
		else
		{
			m_currentState = STATE::ARRIVAL;
			m_circleShape.setFillColor(sf::Color::Red);
		}
	}
	if ((m_currentState & STATE::ARRIVAL) == STATE::ARRIVAL)
	{
		desiredForce += Arrival(cursorPos, 20, 5, perc);
	}
	if ((m_currentState & STATE::OBSTACLEAVOID) == STATE::OBSTACLEAVOID)
	{
		desiredForce += ObstacleAvoid(m_obstacleVec[0], 30, 50);
	}
	if ((m_currentState & STATE::HIVEMIND) == STATE::HIVEMIND)
	{
		for (int i = 0; i < vecBoid.size(); ++i)
		{
			desiredForce += Seek(vecBoid[i].m_position, 1);
		}
	}
	if ((m_currentState & STATE::FLOCK) == STATE::FLOCK)
	{
		desiredForce += Seek(cursorPos, 3);
		Vector2D averageDirection = Vector2D(0);
		int c = 0;
		for (int i = 0; i < vecBoid.size(); ++i)
		{
			if ((vecBoid[i].m_position - m_position).Magnitude() < 75.0f)
			{
				averageDirection += vecBoid[i].m_direction;
				desiredForce += Flee(vecBoid[i].m_position, 50, 5);
				c = i;
			}
		}
		if (c > 0)
		{
			averageDirection /= c;
		}
		m_direction += averageDirection;
	}

	for (int i = 0; i < vecBoid.size(); ++i)
	{
		desiredForce += Flee(vecBoid[i].m_position, 50, 10.0f);
	}

	Vector2D a = (desiredForce + (m_direction * m_speed) / m_mass) * perc;
	if (perc > 0.0f)
	{
		if (a.Magnitude() > 0)
		{
			m_direction = a;
			m_direction.Normalize();
			m_position += a * DeltaTime;
		}
	}
	ScreenWrapping();
}

void DrawLines(Vector2D start, Vector2D end)
{
	g_lines[0].position = sf::Vector2f(start.x, start.y);
	g_lines[1].position = sf::Vector2f(end.x, end.y);
	g_lines[0].color = sf::Color::Cyan;
	g_lines[1].color = sf::Color::Cyan;
}

// screen wrapping for the boid instance
void Boid::ScreenWrapping()
{
	if (m_position.x > 1920) { m_position.x = 0; }
	if (m_position.x < 0) { m_position.x = 1920; }
	if (m_position.y > 1080) { m_position.y = 0; }
	if (m_position.y < 0) { m_position.y = 1080; }
}

// prediction of future cursor location
Vector2D Boid::SimplePredict(Vector2D currPos, Vector2D prevPos)
{
	Vector2D changeVector = Vector2D(currPos - prevPos);
	float magnitude = changeVector.Magnitude();
	if (magnitude > 0) { changeVector.Normalize(); }
	return currPos + changeVector * magnitude * 100.0f;
}

// render screen
void Boid::Render(std::shared_ptr<sf::RenderWindow> window)
{
	m_circleShape.setPosition(m_position.x, m_position.y);
	m_circleShape.setRotation((m_direction.VecAngle() + (3.1416 / 2)) * (180 / 3.1416));
	window->draw(m_circleShape);
	window->draw(g_lines);
}

// seek behaviour
Vector2D Boid::Seek(Vector2D pos, float magnitude)
{
	Vector2D force = pos - m_position;
	if (force.Magnitude() > 0)
	{
		force.Normalize();
		force *= magnitude;
		return force;
	}
	else
	{
		return Vector2D(0);
	}
}

// flee behaviour
Vector2D Boid::Flee(Vector2D pos, float action_radio, float magnitude)
{
	Vector2D force = m_position - pos;
	float magn = force.Magnitude();
	if (magn < action_radio && magn > 0)
	{
		if (magn > 0)
		{
			force.Normalize();
			force *= magnitude;
			return force;
		}
		else { return Vector2D(0); }
	}
	return Vector2D(0);
}

// pursuit behaviour
Vector2D Boid::Pursuit(Vector2D currPos, Vector2D prevPos, float vel, bool isCursor)
{
	if (isCursor) { return Seek(SimplePredict(currPos, prevPos), vel); }
	return Vector2D(0);
}

// evade behaviour
Vector2D Boid::Evade(Vector2D currPos, Vector2D prevPos, float action_radio, float magnitude, bool isCursor)
{
	if (isCursor)
	{
		return Flee(SimplePredict(currPos, prevPos), action_radio, magnitude);
	}
	return Vector2D(0);
}

// obstacle avoid behaviour
Vector2D Boid::ObstacleAvoid(Obstacle& obstacle, float magnitude, float tolerance)
{
	if ((m_position - obstacle.GetPosition()).Magnitude() < tolerance)
	{
		Vector2D force = Flee(obstacle.GetPosition(), obstacle.GetRadius() + obstacle.GetRadius() * 0.2f, magnitude);
		return force;
	}
	return Vector2D(0.0f);
}

// set the window to use
void Boid::SetWindow(std::shared_ptr<sf::RenderWindow> window)
{
	m_window = window;
}

// path follow behaviour
Vector2D Boid::PathFollowing(vector<Vector2D>& pathPoints, float magnitude)
{
	if (m_i == pathPoints.size()) { m_i = 0; }
	return Seek(pathPoints[m_i], magnitude);
}

// wander behaviour
Vector2D Boid::Wander(float magnitude, float angle)
{
	float randAngle = rand() % (int)angle;

	float x1 = m_position.x + (m_direction.x - m_position.x) * cos(randAngle) - (m_direction.y - m_position.y) * sin(randAngle);
	float y1 = m_position.y + (m_direction.x - m_position.x) * sin(randAngle) + (m_direction.y - m_position.y) * cos(randAngle);

	return Vector2D(x1, y1);
}

// queue behaviour
Vector2D Boid::Queue(vector<Boid>& vecBoid, float radius, float magnitude, float& perc)
{
	return Arrival(vecBoid[m_ID + 1].m_position, radius, magnitude, perc);
}

// leaderfollowing behaviour
Vector2D Boid::LeaderFollowing(vector<Boid>& vecBoid, float radius, float magnitude, float& perc)
{
	return Arrival(vecBoid[vecBoid.size()-1].m_position, radius, magnitude, perc);
}

// arrival behaviour
Vector2D Boid::Arrival(Vector2D pos, float radius, float magnitude, float& perc)
{
	if (magnitude <= 0.0f)
	{
		return Vector2D(0.0f);
	}
	float distMag = (pos - m_position).Magnitude();
	if (distMag < radius)
	{
		float b = distMag / radius;
		perc = b;
		if (distMag < (radius * 0.1f))
		{
			perc = 0.0f;
		}
	}
	return Seek(pos, magnitude);
}

// avoid all instances
Vector2D Boid::AvoidAll(vector<Boid>& vecBoid, Vector2D desiredForce, float tolerance, float magnitude)
{
	for (int i = 0; i < vecBoid.size(); ++i)
	{
		desiredForce += Flee(vecBoid[i].m_position, tolerance, magnitude);
	}
	return desiredForce;
}
