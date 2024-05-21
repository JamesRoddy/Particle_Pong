#include <SFML/Graphics.hpp>
#include <iostream>

class Paddle
{
private:
	float m_speed = 400.f; 
	sf::Vector2f m_size; // defining the vairbale object as an object of the built in sfml Vector2f class 
	sf::RectangleShape m_shape;
	bool m_hasCollided = false; 

public:
	// defing paddle methods that will be visible across every file 
	Paddle(sf::Vector2f position, float width, float height, sf::Color color); // defining consructor/initialiser function for the paddle class(including its parameters)
	void trackBall(sf::Vector2f ballPos,sf::Vector2f playerPos,float dt,float windowYVal); // defining track ball method(functionality is defined in paddle.ccp)
	void draw(sf::RenderWindow& window); 
	void move(float dt, float windowYVal);
	void reset(sf::Vector2f position);
	sf::FloatRect getBounds() const; // used to get the bounds of the shape object assigned to the m_shape attribute 
	sf::RectangleShape getShape();

	void setSpeed(float speed);
};

