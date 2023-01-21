#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

#include "defs.h"

class Boid
{
public:
	static constexpr float SPEED = 3.f;
	static constexpr float TURN_SPEED = 0.01f;
	static constexpr float EDGE_TURN_SPEED = 0.05f;
	static constexpr float DET_RAD = 50.f;
	static constexpr float DET_ANG = PI * 0.75f;
	static constexpr float ALIGN_FACTOR = 0.0075f;

	Boid()
		: position(0.f, 0.f)
		, direction(0.f, 0.f)
		, colour(sf::Color::White)
	{
	}

	~Boid() = default;

	void update(int idx, Boid* boids)
	{
		sf::Vector2f com(0.f, 0.f);
		int detected = 0;

		for (int i = 0; i < BOID_COUNT; i++) {
			if (i == idx) {
				continue;
			}
			auto& other = boids[i];
			sf::Vector2f to = other.position - position;
			if (len(to) < DET_RAD) {
				float theta = ang(direction, to);
				if (std::abs(theta) < DET_ANG) {
					turn_towards(other.position, -1.f);
					align_with(other);
					com += other.position;
					detected++;
				}
			}
		}

		if ((com.x != 0.f || com.y != 0.f) && detected != 0) {
			com.x /= static_cast<float>(detected);
			com.y /= static_cast<float>(detected);
			turn_towards(com, 1.f);
		}

		position += SPEED * direction;

		if (position.x < 0.f) {
			float theta = ang(direction, { -1.f, 0.f });
			turn_by(-theta * EDGE_TURN_SPEED);
		}

		if (position.y < 0.f) {
			float theta = ang(direction, { 0.f, -1.f });
			turn_by(-theta * EDGE_TURN_SPEED);
		}

		if (position.x > WINDOW_WIDTH) {
			float theta = ang(direction, { 1.f, 0.f });
			turn_by(-theta * EDGE_TURN_SPEED);
		}

		if (position.y > WINDOW_HEIGHT) {
			float theta = ang(direction, { 0.f, 1.f });
			turn_by(-theta * EDGE_TURN_SPEED);
		}

		colour = from_hsv(std::atan2f(direction.y, direction.x)*180.f/PI + 180, .4f, 1.f);
	}

	void draw(sf::RenderWindow& window) const
	{
		const float angle = std::atan2f(direction.y, direction.x)*180.f/PI + 90;

		sf::ConvexShape shape(3);
		shape.setPoint(0, sf::Vector2f( 0.0f, -0.65f));
		shape.setPoint(1, sf::Vector2f(-0.5f,  0.65f));
		shape.setPoint(2, sf::Vector2f( 0.5f,  0.65f));

		shape.setFillColor(colour);
		shape.setScale(7.f, 7.f);
		shape.setRotation(angle);
		shape.setPosition(position);

		window.draw(shape);
	}

	sf::Vector2f position;
	sf::Vector2f direction;
	sf::Color colour;

private:
	void turn_towards(const sf::Vector2f& point, float dir) { turn_by(ang(direction, point - position) * dir * TURN_SPEED); }
	void align_with(const Boid& other) { turn_by(ang(direction, other.direction) * ALIGN_FACTOR); }
	void turn_by(float theta) { direction = norm(rot(direction, theta)); }
	float len(const sf::Vector2f& v) { return std::sqrtf(v.x*v.x + v.y*v.y); }
	float ang(const sf::Vector2f& v, const sf::Vector2f& w) { return std::atan2(v.x*w.y - v.y*w.x, v.x*w.x + v.y*w.y); }
	sf::Vector2f norm(const sf::Vector2f& v) { return v / len(v); }
	sf::Vector2f rot(const sf::Vector2f& v, float theta) { return { v.x*std::cosf(theta) - v.y*std::sinf(theta), v.x*std::sin(theta) + v.y*std::cosf(theta) }; }

	sf::Color from_hsv(float hue, float sat, float val)
	{
		float C = sat * val;
		float X = C * (1 - std::abs(std::fmodf(hue / 60.0f, 2.f) - 1));
		float m = val - C;

		float r, g, b;
		r = g = b = 0.0f;

		if (0 <= hue && hue < 60)
		{
			r = C;
			g = X;
			b = 0.0f;
		}
		else if (60 <= hue && hue < 120)
		{
			r = X;
			g = C;
			b = 0.0f;
		}
		else if (120 <= hue && hue < 180)
		{
			r = 0.0f;
			g = C;
			b = X;
		}
		else if (180 <= hue && hue < 240)
		{
			r = 0.0f;
			g = X;
			b = C;
		}
		else if (240 <= hue && hue < 300)
		{
			r = X;
			g = 0.0f;
			b = C;
		}
		else if (300 <= hue && hue < 360)
		{
			r = C;
			g = 0.0f;
			b = X;
		}

		return sf::Color(
			static_cast<char>((r + m) * 255.0f),
			static_cast<char>((g + m) * 255.0f),
			static_cast<char>((b + m) * 255.0f)
		);
	}
};
