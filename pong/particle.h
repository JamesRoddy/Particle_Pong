#include <SFML/Graphics.hpp>
#include<vector>


class particle {
public:

	particle(float x, float y, sf::Color, float radius);
	void update(float dt);
	sf::CircleShape particleShape;
	sf::Vector2f velocity;
	float particleX;
	float particleY;
private:
	float speed = 3;
	float alpha = 1;






};

class effectGenerator {
public:
	effectGenerator(unsigned int width, unsigned int height);
	void update(float dt);
	void draw(sf::RenderWindow& window);
	void generateParticles(int newCount, float radius);

private:
	std::vector<particle> currentParticles;
	unsigned int windowWidth = 0;
	unsigned int windowHeight = 0;

};