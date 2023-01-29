#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "defs.h"

class Quadtree
{
	struct Node
	{
		sf::FloatRect region;
		std::vector<Boid> inhabitants;
		std::vector<Node> children;

		Node(const sf::FloatRect& r)
			: region(r)
			, inhabitants()
			, children()
		{
		}
	};

public:
	static constexpr unsigned THRESHOLD = 10;

	Quadtree(const std::vector<Boid>& boids)
		: m_root({0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT})
		, m_boids(boids)
	{
	}

	void subdivide()
	{
		m_root.inhabitants = m_boids;
		subdivide(m_root);
	}

	std::vector<Boid> get_possible(float x, float y) { return get_possible(m_root, x, y); }

private:
	std::vector<Boid> get_possible(const Node& n, float x, float y)
	{
		if (n.inhabitants.size() < THRESHOLD) {
			return n.inhabitants;
		}

		for (auto& ch : n.children) {
			if (ch.region.contains(x, y)) {
				return get_possible(ch, x, y);
			}
		}

		return { };
	}

	void subdivide(Node& n)
	{
		auto rw = n.region.width / 2;
		auto rh = n.region.height / 2;
		auto rx = n.region.left;
		auto ry = n.region.top;

		sf::FloatRect r0(rx     , ry     , rw, rh);
		sf::FloatRect r1(rx + rw, ry     , rw, rh);
		sf::FloatRect r2(rx     , ry + rh, rw, rh);
		sf::FloatRect r3(rx + rw, ry + rh, rw, rh);

		n.children.clear();

		Node n0(r0);
		Node n1(r1);
		Node n2(r2);
		Node n3(r3);

		for (auto& b : n.inhabitants) {
			if (n0.region.contains(b.position)) n0.inhabitants.push_back(b);
			if (n1.region.contains(b.position)) n1.inhabitants.push_back(b);
			if (n2.region.contains(b.position)) n2.inhabitants.push_back(b);
			if (n3.region.contains(b.position)) n3.inhabitants.push_back(b);
		}

		if (n0.inhabitants.size() > THRESHOLD) subdivide(n0);
		if (n1.inhabitants.size() > THRESHOLD) subdivide(n1);
		if (n2.inhabitants.size() > THRESHOLD) subdivide(n2);
		if (n3.inhabitants.size() > THRESHOLD) subdivide(n3);

		n.children.push_back(n0);
		n.children.push_back(n1);
		n.children.push_back(n2);
		n.children.push_back(n3);
	}

	Node m_root;
	const std::vector<Boid>& m_boids;
};
