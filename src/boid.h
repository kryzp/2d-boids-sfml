#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

#include "defs.h"

class Boid
{
public:
	static constexpr float SPEED = 1.f;
	static constexpr float TURN_SPEED = 0.0002f;
	static constexpr float DET_DST = 25.f;
	static constexpr float DET_ANG = PI * 0.75f;
	static constexpr float ALIGN_FACTOR = 0.0075f;

	Boid()
		: position(0.f, 0.f)
		, direction(0.f, 0.f)
		, colour(sf::Color::Green)
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
			if (len(to) < DET_DST) {
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
		position.x = wrap(position.x, 0.f, WINDOW_WIDTH);
		position.y = wrap(position.y, 0.f, WINDOW_HEIGHT);
	}

	void draw(sf::RenderWindow& window) const
	{
		const float angle = std::atan2f(direction.y, direction.x)*180.f/PI + 90;

		sf::ConvexShape shape(3);
		shape.setPoint(0, sf::Vector2f( 0.0f, -0.65f));
		shape.setPoint(1, sf::Vector2f(-0.5f,  0.65f));
		shape.setPoint(2, sf::Vector2f( 0.5f,  0.65f));

		shape.setFillColor(colour);
		shape.setScale(5.f, 5.f);
		shape.setRotation(angle);
		shape.setPosition(position);

		window.draw(shape);
	}

private:
	void turn_towards(const sf::Vector2f& point, float dir)
	{
		float theta = ang(direction, point - position);
		direction = norm(rot(direction, dir * theta * TURN_SPEED));
	}

	void align_with(const Boid& other)
	{
		float theta = ang(direction, other.direction);
		direction = norm(rot(direction, theta * ALIGN_FACTOR));
	}

	float wrap(float v, float mn, float mx) { return (v > mx) ? mn+10.0f : (v < mn) ? mx-10.0f : v; }
	float len(const sf::Vector2f& v) { return std::sqrtf(v.x*v.x + v.y*v.y); }
	float ang(const sf::Vector2f& v, const sf::Vector2f& w) { return std::atan2(v.x*w.y - v.y*w.x, v.x*w.x + v.y*w.y); }
	sf::Vector2f norm(const sf::Vector2f& v) { return v / len(v); }
	sf::Vector2f rot(const sf::Vector2f& v, float theta) { return { v.x*std::cosf(theta) - v.y*std::sinf(theta), v.x*std::sin(theta) + v.y*std::cosf(theta) }; }

public:
	sf::Vector2f position;
	sf::Vector2f direction;
	sf::Color colour;
};
