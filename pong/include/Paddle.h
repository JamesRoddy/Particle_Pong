#include <SFML/Graphics.hpp>
#include <iostream>

class Paddle
{
private:
	float m_speed = 400.f;
	sf::Vector2f m_size; // defining the vairbale object as an object of the built in sfml Vector2f class 
	sf::RectangleShape m_shape;

public:
	Paddle(sf::Vector2f position, float width, float height, sf::Color color); // defining consructor/initialiser function for the paddle class(including its parameters)
	void trackBall(sf::Vector2f ballPos,float dt,float windowYVal); // defining track ball method(functionality is defined in paddle.ccp)
	void hasCollided(sf::Vector2f ballPos);
	void draw(sf::RenderWindow& window); 
	void move(float dt, float windowYVal);
	sf::FloatRect getBounds() const;
	sf::RectangleShape getShape();

	void setSpeed(float speed);
};

