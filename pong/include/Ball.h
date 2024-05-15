#include <SFML/Graphics.hpp>

class Ball
{
private:
	float m_speed;
	float m_maxSpeed;
	float m_speedIncreaseMultiplier;
	sf::CircleShape m_shape;
	sf::Vector2f m_velocity;
public:
	Ball(sf::Vector2f position, float radius, float speed, sf::Color color);

	void increaseSpeed(float dt);
	void draw(sf::RenderWindow& window);
	void move(float dt, sf::RenderWindow& window);
	sf::Vector2f getPosition();
	void setPosition(float x, float y);
	void resetPos(float newDirection,int newX,int newY);
	void updateVelocity(float val);
	sf::CircleShape getShape();
};

