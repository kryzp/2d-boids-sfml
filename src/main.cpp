#include <SFML/Graphics.hpp>

#include "boid.h"
#include "quad_tree.h"
#include "defs.h"

int main()
{
	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Boids");

	std::vector<Boid> boids;

	for (int i = 0; i < powf(2.0f, 13.0f); i++) {
		Boid b;
		b.position  = sf::Vector2f(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
		b.direction = sf::Vector2f(rand() % 100 - 50, rand() % 100 - 50);
		boids.push_back(b);
	}

	Quadtree space(boids);

	sf::Clock deltaClock;
	float dt;

	while (window.isOpen())
	{
		dt = deltaClock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		space.subdivide();

		window.clear(sf::Color(11, 13, 15));

		for (int i = 0; i < boids.size(); i++) {
			boids[i].update(i, space.get_possible(boids[i].position.x, boids[i].position.y), dt);
			boids[i].draw(window);
		}

		window.display();
	}

	return 0;
}
