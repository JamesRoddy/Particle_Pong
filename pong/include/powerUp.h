#pragma once
#include "Ball.h"
#include "Paddle.h"
#include <SFML/Graphics.hpp>

class powerUp{

public: 
	
	powerUp(float windowWidth, float windowHeight);
	sf::Vector2f randomTarget();
	void drawPowerUp(sf::RenderWindow& window);
	bool collision(sf::FloatRect bounds);
	void applyEffect(Ball*ball, float dt);
	sf::RectangleShape getShape();

	void updatePowerUpPos(float dt);

	void setType(sf::Color colour, sf::Time effectTime);
	void incrementDuration();

	bool negateEffect(Ball*ball);
	sf::Time getDuration();
	void setTimeOfCollision(sf::Time collisionTime);
	sf::Time getTotalEffectTime();
	void setTotalEffectTime(sf::Time newEffectTime);
	sf::Vector2f scaleObjectSize(sf::Vector2f intitalSize);
	void setPaddle(Paddle* newPaddleRef);
	int getTargetHitCount();
	int getMaxTargetHitCount();
	void setSpawn();
	int getId();
	
private:
	enum effects { INVERTVELOCITY, INCREASEBALLSPEED, INCREASEPADDLESIZE };
	static const int powerUpTypes = 3;
	
	int m_targetHitCount;
	int m_maxTargetHitCount;

	Paddle* paddleRef ;
	float m_powerUpScaleFactor;
	float m_ballSpeedMultiplier ;
	float m_speed;
	float m_windowWidth;
	float m_windowHeight;
	int m_id;
	sf::Color m_colour;
	sf::Time m_totalEffectTime;
	sf::Time m_currentEffectTime;

	sf::RectangleShape m_shape;
	sf::Vector2f m_target;
	sf::Time m_tickUpTime;
	bool m_hasCollided = false;
};