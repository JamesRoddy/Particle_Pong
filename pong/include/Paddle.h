#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Paddle
{
private:
	float m_speed = 400.f;  // original speed for paddle to move by(also used to set velocity)
	sf::Vector2f m_size; // defining the vairbale object as an object of the built in sfml Vector2f class 
	sf::RectangleShape m_shape; // definig the built in sfml rectangle shape object that is associated with the paddle 
	float m_aISpeedMultiplier; // speed multipler for the ai meaning it willl adjust its speed based on target distance
	float m_baseAiSpeedMultiplier;
	sf::Vector2f m_aITarget; // used to set where the ai should move 
	// keep track of previous scores used to control ai speed
	float m_lastScoreCheckPlayer;
	float m_lastScoreCheckAi; 
	// used to adjust ai speed based on score
	float m_aiSpeedController;
	float m_maxAiSpeed;
	float m_minAiSpeed;

	
	void AiMovement(sf::Vector2f target, float fWindowYVal, float dt, sf::Vector2f fBallPos); // used to move ai based on a target position
	void checkBounds(float fWindowYVal); // ensure that paddles are kpet in bounds of screen
	float getRayIntersectionValue(sf::Vector2f fBalVelocity, sf::Vector2f fBallPos, float fWindowYVal, sf::Vector2f fLineTop, sf::Vector2f lineBottom); // used for ai movement allowing it to move in a more relatsict fashion i.e it makes use of a target point rather than just the ball position
	sf::Vector2f lerpToIntersection(sf::Vector2f fStart, sf::Vector2f fEnd, float fPercent, float fWindowYVal); // used to find where the ball will hit the edge of the screen on the ai paddle's side



public:
	// defing paddle methods that will be visible across every file 
	Paddle(sf::Vector2f fPosition, float fWidth, float fHeight, sf::Color color); // defining consructor/initialiser function for the paddle class(including its parameters)

	void draw(sf::RenderWindow& window) const; // drawing paddle to screen
	void move(float fDt, float fWindowYVal); // moving the player paddle up and down using dt to control the rate of movement and the y value to consrtict the movement of the player
	void reset(sf::Vector2f fPosition); // used to reset the paddle position across game states

	void trackBall(sf::Vector2f fBallPos, sf::Vector2f fBallVelocity, float fDt, float fWindowYVal); // defining track ball method(functionality is defined in paddle.ccp)
	void aiValidateScore(float playerScore,float aiScore,float iMaxScore);

	sf::FloatRect getBounds() const; // used to get the bounds of the shape object assigned to the m_shape attribute 
	sf::RectangleShape getShape();
	sf::RectangleShape* getShapeReference();
	
	void setSpeed(float fSpeed); // used to set the speed of the paddle 
};

