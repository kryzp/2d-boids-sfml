#include <SFML/Graphics.hpp>

#include "boid.h"
#include "defs.h"

int main()
{
	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Boids");

	Boid boids[BOID_COUNT];

	for (auto& b : boids) {
		b.position  = sf::Vector2f(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
		b.direction = sf::Vector2f(rand() % 100 - 50, rand() % 100 - 50);
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		for (int i = 0; i < BOID_COUNT; i++) {
			boids[i].update(i, boids);
		}

		window.clear();

		for (auto& b : boids) {
			b.draw(window);
		}

		window.display();
	}

	return 0;
}
