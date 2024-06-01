#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Paddle
{
private:
	float m_speed = 400.f; 
	sf::Vector2f m_size; // defining the vairbale object as an object of the built in sfml Vector2f class 
	sf::RectangleShape m_shape;
	float m_aISpeedMultiplier;
	sf::Vector2f m_aITarget;



public:
	// defing paddle methods that will be visible across every file 
	Paddle(sf::Vector2f position, float width, float height, sf::Color color); // defining consructor/initialiser function for the paddle class(including its parameters)

	void draw(sf::RenderWindow& window); // drawing paddle to screen
	void move(float dt, float windowYVal); // moving the player paddle up and down using dt to control the rate of movement and the y value to consrtict the movement of the player
	void reset(sf::Vector2f position); // used to reset the paddle position across game states
	void trackBall(sf::Vector2f ballPos, sf::Vector2f velocity, float dt, float windowYVal, float fwindowXVal,float ballRadius); // defining track ball method(functionality is defined in paddle.ccp)
	void AiMovement(sf::Vector2f target,float windowYVal,float dt,sf::Vector2f ballPos); // used to move ai based on a target position
	
	float getRayIntersectionValue(sf::Vector2f velocity,sf::Vector2f ballPos,float windowYVal,sf::Vector2f lineTop,sf::Vector2f lineBottom); // used for ai movement allowing it to move in a more relatsict fashion i.e it makes use of a target point rather than just the ball position
	

	sf::Vector2f lerpToIntersection(sf::Vector2f start, sf::Vector2f end, float percent, float windowYval); // used to find where the ball will hit the edge of the screen on the ai paddle's side
	sf::FloatRect getBounds() const; // used to get the bounds of the shape object assigned to the m_shape attribute 
	sf::RectangleShape getShape();
	sf::RectangleShape* getShapeReference();
	
	void setSpeed(float speed); // used to set the speed of the paddle 
};

