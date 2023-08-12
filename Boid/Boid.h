#pragma once
#include "Vector2D.h"
#include <vector>
#include "Obstacle.h"
#include <ctime>
#include <cstdlib>
#include <SFML/Graphics.hpp>

using std::shared_ptr;
using std::vector;

enum STATE
{
	SEEK = 1,
	FLEE = 1 << 1, // bit shift
	PURSUIT = 1 << 2,
	EVADE = 1 << 3,
	PATHFOLLOW = 1 << 4,
	WANDER = 1 << 5,
	QUEUE = 1 << 6,
	LEADERFOLLOW = 1 << 7,
	ARRIVAL = 1 << 8,
	OBSTACLEAVOID = 1 << 9,
	HIVEMIND = 1 << 10,
	FLOCK = 1 << 11
};

class Boid
{
public:
	// test
	int m_checkCurs = 0;
	Vector2D m_prevCursPos;

	int m_ID;
	int m_i = 0;
	float m_mass;
	float m_speed;
	Vector2D m_position;
	Vector2D m_direction;
	sf::CircleShape m_circleShape;
	std::shared_ptr<sf::RenderWindow> m_window;
	int m_currentState;
	std::vector<Vector2D> m_pathPoints;
	std::vector<Obstacle> m_obstacleVec;
	float m_timer;
	int m_currentPos;
	float m_visualCone;

	Boid();
	Boid(Vector2D position, Vector2D direction = Vector2D::FRONT, float radius = 10.0f, int currentState = 0, float visualRange = 45.0f, float speed = 50, int ID = 0, float mass = 2.0f);
	~Boid();

	// base
	//void Start();
	void Update(float DeltaTime, vector<Boid>& vecBoid);
	void Render(std::shared_ptr<sf::RenderWindow> window);
	void SetWindow(std::shared_ptr<sf::RenderWindow> window);
	void ScreenWrapping();
	Vector2D SimplePredict(Vector2D currPos, Vector2D prevPos);

	// steering behaviours
	Vector2D Seek(Vector2D pos, float magnitude);
	Vector2D Flee(Vector2D pos, float action_radio, float magnitude);
	Vector2D Pursuit(Vector2D currPos, Vector2D prevPos, float vel, bool isCursor);
	Vector2D Evade(Vector2D currPos, Vector2D prevPos, float action_radio, float magnitude, bool isCursor);
	Vector2D ObstacleAvoid(Obstacle& obstacle, float magnitude, float tolerance);
	Vector2D PathFollowing(vector<Vector2D>& pathPoints, float magnitude);
	Vector2D Wander(float magnitude, float angle);
	Vector2D Queue(vector<Boid>& vecBoid, float radius, float magnitude, float& perc);
	Vector2D LeaderFollowing(vector<Boid>& vecBoid, float radius, float magnitude, float& perc);
	Vector2D Arrival(Vector2D pos, float radius, float magnitude, float& perc);

	Vector2D AvoidAll(vector<Boid>& vecBoid, Vector2D desiredForce, float tolerance, float magnitude);
};

