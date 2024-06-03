#pragma once
#include <SFML/Graphics.hpp>

class Ball
{

public:
	Ball(sf::Vector2f fPosition, float fRadius, float fSpeed, sf::Color color); // constrctor for the ball allowing values to be assgined to its attributes 
	
	bool ballCollisionPushBack(sf::RectangleShape paddleBounds,float fDt);
	void increaseSpeed(float fDt); // method to increase the balls speed over time 
	float getRotation(float fDistanceToCentre, sf::RectangleShape PaddleBounds); // used to determine how the balll should bounce of the paddle
	
	void draw(sf::RenderWindow& window); // method to redner the ball to the window using a refernce argument ot the current sf::render window object 
	void move(float fDt, sf::RenderWindow& window); // method used to move the ball and keep it in the bounds of the screen 
	
	sf::CircleShape* getShapeReference();
	sf::Vector2f getPosition(); // returns an sf::vector2f object that identifies the current position of the ball
	void setPosition(float fX, float fY); // setting the position of the ball using the passed in x and y float variables  
	
	void resetPos(int iNewX,int iNewY); // used for resetting the balls speed, velocity, and postion each time it goes off screen
	void updateVelocity(float fVal); // used to switch the direction of the ball
	void setVelocity(sf::Vector2f fNewVelcoity);
	float getSpeed();
	void setSpeed(float fNewSpeedValue);
	sf::Vector2f getVelocity();
	sf::CircleShape getShape(); // returns the m_shape attribute that is an object of class CircleShape(built into sfml)
	
	
	// determine the private attributes of the ball, these will only be accessible by objects of this class 
private:
	float m_speed; // determine how fast the ball moves
	float m_maxSpeed; // used to control the overall speed limit the ball has 
	float m_speedIncreaseMultiplier; // used to control at what rate the ball will increase in speed each collision 
	
	float m_maxCollsionAngle; // maximum rotation angle that can be applied to the ball when it hits paddle
	sf::CircleShape m_shape; // the ball has an atttribute that is an object of the built in CircleShape class 
	sf::Vector2f m_velocity; // will determine the direction of movement for the ball

};

