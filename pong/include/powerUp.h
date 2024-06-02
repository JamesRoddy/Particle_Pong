#pragma once
#include "Ball.h"
#include "Paddle.h"
#include <SFML/Graphics.hpp>

class powerUp{

public: 
	
	powerUp(float fWindowWidth, float fWindowHeight); // intial constcor for power up that sets its properties for the sf::rectangle shape object attribute of the power up
	sf::Vector2f randomTarget(); // used to set a ranodm target for the power up each time it reaches its current movement target
	void drawPowerUp(sf::RenderWindow& window); // draw power ups to screen
	bool collision(sf::FloatRect fBounds); // used to detect collsion between power up and ball
	void applyEffect(Ball*ball, float fDt); // apply the power up effects to objects 
	sf::RectangleShape getShape(); // return the sf rectangle shape object attribute of the paddle allowing for manipluation of it outside of the power up class

	void updatePowerUpPos(float fDt); // used to update the power ups current postion based on its target

	void setType(sf::Color colour, sf::Time effectTime); // used to set the type of the power up based on its randomly geenrated id 
	void incrementDuration(); // increment the duration effect of the power up if it is currenlty active and the ball hits a power up of the same type as it  

	bool negateEffect(Ball*ball); // reset any duration effects the power up applied to objects
	void setTimeOfCollision(sf::Time collisionTime); // get the time of collsion between the ball and the power up object
	void setTotalEffectTime(sf::Time newEffectTime); // used to set the total effect time of the power up when needed
	void setPaddle(Paddle* newPaddleRef, int direction); // used to set the paddleRefernce attribute  to point to the memeory loctaion of either the paddle or the ai based on the ball direction
	void setObjectTarget();
	int getEffectDirection();
	int getTargetHitCount();
	int getMaxTargetHitCount();
	bool hasBall();
	void setSpawn();
	int getId(); 
	
private:
	enum effects { INVERTVELOCITY, INCREASEBALLSPEED, INCREASEPADDLESIZE }; // a set of enum constants that are used to help keep track of power up effects
	
	// used to detrmine how many times the power up can move before it fades off screen
	int m_targetHitCount;
	int m_maxTargetHitCount;
	bool m_hasPaddleEffect = false;
	bool m_hasBallEffect = false;
	int m_direction ;
	Paddle* m_paddleRef ; // refernce to the paddle any effects need to be applied to 
	//power up effects
	float m_powerUpScaleFactor;
	float m_ballSpeedMultiplier ; 

	float m_speed; // speed power up moves on the screen9used to set velocity)
	float m_windowWidth;// window widht and height used to keep the power up on screen while it is moving
	float m_windowHeight; 
	int m_id; // the id associated with the power up that determines its effects
	sf::Color m_colour; 
	sf::Time m_totalEffectTime; // total effect time that the power up has
	sf::Time m_currentEffectTime; // the current effect time of the power up 

	sf::RectangleShape m_shape; //  the sfml rectangle object asosctaied with the power up  
	sf::Vector2f m_target; // used to set the movemetn target for the power up allowing it to move around on screen
	sf::Time m_tickUpTime; // time that the current duration effect will increase by until it reaches its maximum rotation
};