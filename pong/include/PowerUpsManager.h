#pragma once

#include "powerUp.h"
#include<vector>

class powerUpsManager {

public:
	powerUpsManager(float windowWidth,float windowHeight);
	void generatePowerUp();
	void handleCollision(Ball*ball,Paddle*player,Paddle*AI);
	void draw(sf::RenderWindow &window);
	void clearPowerUps(Ball*ball);

	void resetTimers();
	void manageDurationEffects(Ball*ball,float dt);
	void update(float dt);
	bool searchForExistingEffect(powerUp &newPowerUp);
	
private:
	
	std::vector<powerUp>m_powerUps; // keep track of what power ups should be drawn and moved on the screen
	std::vector<powerUp> m_activePowerUp; // keep track of active power up effects
	static const int powerUpTypes = 3; // number of power up types that can spawn
	sf::Time m_effectDurations[powerUpTypes] = { sf::seconds(0.0f),sf::seconds(0.5f),sf::seconds(5.0f) }; // effect durations associated with powerUps
	sf::Color m_powerUpColours[powerUpTypes] = { sf::Color::Magenta,sf::Color::Blue,sf::Color::Red }; // colours assoctaed with powerUps
	
	sf::Clock m_eventTimer; // used to get the elapsed time since the game started
	sf::Clock m_CollisionTimer; // used to provide power ups with a time of collsion allowing for effect duration and negation
	sf::Time m_currentEventTime; /// keep track of when powerUp should spawn
	sf::Time m_powerUpGenTime; /// time for when powerups should spawn 
	float m_windowWidth; 
	float m_windowHeight;
	
};