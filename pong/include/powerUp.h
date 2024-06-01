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
	void incrementDuration(sf::Time newDuration);

	bool negateEffect(Ball*ball, float dt);
	sf::Time getDuration();
	void setTimeOfCollision(sf::Time collisionTime);
	sf::Time getTotalEffectTime();
	
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
	float m_ballSpeedMultiplier = 50.0f;
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