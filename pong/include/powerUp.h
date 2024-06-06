#pragma once
#include "Ball.h"
#include "Paddle.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
class PowerUp{

public: 
	
	PowerUp(float fWindowWidth, float fWindowHeight); // intial constcor for power up that sets its properties for the power up object
	void drawPowerUp(sf::RenderWindow& window); // draw power ups to screen
	bool collision(sf::FloatRect fBounds); // used to detect collsion between power up and ball
	void applyEffect(Ball*ball, float fDt); // apply the power up effects to objects 

	void updatePowerUpPos(float fDt); // used to update the power ups current postion based on its target

	void setType(sf::Color colour, sf::Time effectTime,std::string powerUpText); // used to set the type of the power up based on its randomly geenrated id(used to assign power up properties in the power ups manager class)
	void incrementDuration(); // increment the duration effect of the power up if it is currenlty active and the ball hits a power up of the same type as it  

	bool negateEffect(Ball*ball); // reset any duration effects the power up applied to objects
	void setTotalEffectTime(sf::Time newEffectTime); // used to set the total effect time of the power up when needed
	void setPaddle(Paddle* newPaddleRef, int iDirection); // used to set the paddleReference attribute  to point to the memeory loctaion of either the paddle or the ai based on the ball direction
	
	int getEffectDirection() const; // get the direction the ball was going on collision with the power up
	int getTargetHitCount() const; // get the current target hit count of the power up
	int  getMaxTargetHitCount() const; // get the maxiumum number of successfull movements a power up can make before being erased
	bool hasBall() const; /// used to determine if the power up effect is being applied to the paddle or the ball in the power up manager class 
	void setSpawn(); // set the spawn location of the power up within a specifc bound area
	int getId() const;  // get the id of the power up that is used to determine its effects in the power up manager class
	sf::RectangleShape getShape();
	sf::Text *getPopUpTextRef(); // used to maniplate the pop up text of the power up
	sf::Text getPopUpTextValue();
private:

	sf::Vector2f randomTarget(); // used to set a ranodm target for the power up each time it reaches its current movement target
	void setEffectTarget(); // used to determine which paddle the power up effect is applied to 
	enum effects { INVERTVELOCITY, INCREASEBALLSPEED, INCREASEPADDLESIZE }; // a set of enum constants that are used to help keep track of power up effects
	sf::SoundBuffer m_powerUpSoundBuffer;
	sf::Sound m_powerUpSoundEffect;
	// used to detrmine how many times the power up can move before it fades off screen
	int m_targetHitCount;
	int m_maxTargetHitCount;
	sf::Text m_powerUpText; // text that will get set to the pop up text for the power up when it is hit
	// determine what objects the power up effect should be applied to 
	bool m_hasPaddleEffect;
	bool m_hasBallEffect;
	int m_direction; // dtermine direection of the ball when the power up object collided with it 
	Paddle* m_paddleRef ; // refernce to the paddle any effects need to be applied to 
	//different scale factors used for power up effects 
	float m_powerUpScaleFactor;
	float m_ballSpeedMultiplier ; 
	
	float m_durationScalar;// used to determine how much the power up has its duration increased when it is active 
	float m_speed; // speed  the power up moves on the screen 
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