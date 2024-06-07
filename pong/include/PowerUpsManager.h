#pragma once

#include "powerUp.h"
#include<vector>

class PowerUpsManager {

public:
	PowerUpsManager(sf::Vector2f fWindowDimensions,std::string sPowerUpFontPath); // initial constcror for the power up manager class that takes in the window width and height along with the font path that willbe assigened to the power up pop up text
	void generatePowerUp(); // used to control when power ups should appear on screen
	void handleCollision(Ball*ball,Paddle*player,Paddle*AI); // handinling collsion and assiging any object refernces the power up needs when collision takes place
	void draw(sf::RenderWindow &window); // draw all power ups 
	void clearPowerUps(Ball*ball); // clear all power ups and negate their effects 

	void resetTimers(); // reset the timers 
	void manageDurationEffects(Ball*ball,float fDt); // loop through all active power ups and apply/neagte their effects if needed
	void update(float fDt); // update all power ups
	void addPowerUpText(unsigned int uiCharacterSize,PowerUp &newPowerUp); // used to add pop up text to the m_activePowerUpText vector allowing the pop up text to be drawn to the screens
	void updatePopUpText(float dt);
	void drawPowerUpText(sf::RenderWindow& window);
	// used to keep track of all the effects and whihc effects should have their duration increased based on the power up and its effects
	void searchForExistingBallEffect(PowerUp &newPowerUp);
	void searchForExistingPaddleEffect(PowerUp& newPowerUp);
	
private:

	std::vector<PowerUp>m_powerUps; // keep track of what power ups should be drawn and moved on the screen
	std::vector<PowerUp> m_activePowerUp; // keep track of active power up effects
	std::vector<sf::Text> m_activePopUpText;

	static const int iPowerUpTypes = 3; // number of power up types that can spawn
	sf::Time m_effectDurations[iPowerUpTypes] = { sf::seconds(0.0f),sf::seconds(3.0f),sf::seconds(8.0f) }; // effect durations associated with powerUps
	sf::Color m_powerUpColours[iPowerUpTypes] = { sf::Color::Magenta,sf::Color::Blue,sf::Color::Red }; // colours assoctaed with powerUps
	
	std::string m_powerUpText[iPowerUpTypes] = { "INVERT!","SPEEDUP!","BIG MODE++" }; // store pop up text for power ups
	sf::Font m_powerUpTextFont;
	
	sf::Clock m_eventTimer; // used to get the elapsed time since the game started
	sf::Clock m_CollisionTimer; // used to provide power ups with a time of collsion allowing for effect duration and negation
	sf::Time m_currentEventTime; /// keep track of when powerUp should spawn
	sf::Time m_powerUpGenTime; /// time for when powerups should spawn 
	
	sf::SoundBuffer m_powerUpSoundBuffer;
	sf::Sound m_powerUpSoundEffect;
	float m_textFadeMultiplier;
	float m_windowWidth;  // window width and height used to control where power ups spawn 
	float m_windowHeight;
	
};