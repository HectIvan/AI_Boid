#pragma once
#include <cmath>
#include <cassert>

using std::sqrt;
class Vector2D
{
public:
	Vector2D() = default;
	Vector2D(float _x, float _y = 0.0f)	: x(_x), y(_y) {}
	Vector2D(int _x, int _y = 0.0f) : x(_x), y(_y) {}
	Vector2D(const Vector2D& other) { x = other.x; y = other.y; }
	~Vector2D() = default;

	Vector2D& operator = (const Vector2D& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	// ADD 
	Vector2D operator+(const Vector2D& other)
	{
		return Vector2D(x + other.x, y + other.y);
	}

	Vector2D operator+(const float other)
	{
		return Vector2D(x + other, y + other);
	}

	Vector2D operator+(const int other)
	{
		return Vector2D(x + other, y + other);
	}

	Vector2D operator+=(const Vector2D& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2D operator+=(const float other)
	{
		x += other;
		y += other;
		return *this;
	}

	Vector2D operator+=(const int other)
	{
		x += other;
		y += other;
		return *this;
	}

	// SUBSTRACT
	Vector2D operator-(const Vector2D& other)
	{
		return Vector2D(x - other.x, y - other.y);
	}

	Vector2D operator-(const float other)
	{
		return Vector2D(x - other, y - other);
	}

	Vector2D operator-(const int other)
	{
		return Vector2D(x - other, y - other);
	}

	Vector2D operator-=(const Vector2D& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2D operator-=(const float other)
	{
		x -= other;
		y -= other;
		return *this;
	}

	Vector2D operator-=(const int other)
	{
		x -= other;
		y -= other;
		return *this;
	}

	// MULTIPLY
	Vector2D operator*(const Vector2D& other)
	{
		return Vector2D(x * other.x, y * other.y);
	}

	Vector2D operator*(const float other)
	{
		return Vector2D(x * other, y * other);
	}

	Vector2D operator*(const int other)
	{
		return Vector2D(x * other, y * other);
	}

	Vector2D operator*=(const Vector2D& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vector2D operator*=(const float other)
	{
		x *= other;
		y *= other;
		return *this;
	}

	Vector2D operator*=(const int other)
	{
		x *= other;
		y *= other;
		return *this;
	}

	// DIVIDE
	Vector2D operator/(const Vector2D& other)
	{
		return Vector2D(x / other.x, y / other.y);
	}

	Vector2D operator/(const float other)
	{
		return Vector2D(x / other, y / other);
	}

	Vector2D operator/(const int other)
	{
		return Vector2D(x / other, y / other);
	}

	Vector2D operator/=(const Vector2D& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	Vector2D operator/=(const float other)
	{
		x /= other;
		y /= other;
		return *this;
	}

	Vector2D operator/=(const int other)
	{
		x /= other;
		y /= other;
		return *this;
	}

	float operator|(const Vector2D& other) const
	{
		return (x * other.x) + (y * other.y);
	}

	float DotProd(const Vector2D& other) const
	{
		return this->operator|(other);
	}

	inline float MagnitudeSquare() const
	{
		return x * x + y * y;
	}

	float Magnitude() const
	{
		return sqrt(MagnitudeSquare());
	}

	void Normalize()
	{
		float mag = Magnitude();
		assert(mag > 0.0f && "trying to Normalize a Zero size Vector");
		mag = 1.0f / mag;
		x *= mag;
		y *= mag;
	}

	float VecAngle() const
	{
		return atan2(y, x);
	}

	float LegX(float magnitude, float angle)
	{
		return magnitude * cos(angle);
	}

	float LegY(float magnitude, float angle)
	{
		return magnitude * sin(angle);
	}

	float newX(float magnitude, float angle)
	{
		return x + LegX(magnitude, angle);
	}

	float newY(float magnitude, float angle)
	{
		return y + LegY(magnitude, angle);
	}

public:
	float x, y;

public:
	static const Vector2D FRONT;
	static const Vector2D BACK;
	static const Vector2D UP;
	static const Vector2D DOWN;
};

