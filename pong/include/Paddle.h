#include <SFML/Graphics.hpp>
#include <iostream>

class Paddle
{
private:
	float m_speed = 400.f; 
	sf::Vector2f m_size; // defining the vairbale object as an object of the built in sfml Vector2f class 
	sf::RectangleShape m_shape;
	sf::CircleShape m_debug;
	sf::CircleShape m_debug1;

	float m_aISpeedMultiplier;
	sf::Vector2f m_aITarget;



public:
	// defing paddle methods that will be visible across every file 
	Paddle(sf::Vector2f position, float width, float height, sf::Color color); // defining consructor/initialiser function for the paddle class(including its parameters)

	void draw(sf::RenderWindow& window); 
	void move(float dt, float windowYVal);
	void aiScoreTracker(int playerScore, int aiScore, int maxScore);
	void reset(sf::Vector2f position);
	void trackBall(sf::Vector2f ballPos, sf::Vector2f velocity, float dt, float windowYVal, float fwindowXVal,float ballRadius); // defining track ball method(functionality is defined in paddle.ccp)
	void AiMovement(sf::Vector2f target,float windowYVal,float dt,sf::Vector2f ballPos);
	void aIGenerateResetPosition(float windowYVal);
	float getRayIntersectionValue(sf::Vector2f velocity,sf::Vector2f ballPos,float windowYVal,sf::Vector2f lineTop,sf::Vector2f lineBottom);


	sf::Vector2f lerpToIntersection(sf::Vector2f start, sf::Vector2f end, float percent, float windowYval);
	sf::FloatRect getBounds() const; // used to get the bounds of the shape object assigned to the m_shape attribute 
	sf::RectangleShape getShape();

	void setSpeed(float speed);
};

