#include "PowerUpsManager.h"
#include <iostream>


powerUpsManager::powerUpsManager(float fWindowWidth,float fWindowHeight) {

	m_powerUpGenTime = sf::seconds(2.0f); // time it takes to spawn new power ups 
	m_windowWidth = fWindowWidth; /// window widht and height vairbales used for spawning power ups
	m_windowHeight = fWindowHeight;
	
}


void powerUpsManager::handleCollision(Ball*ball,Paddle*player,Paddle*AI) // pass refernce to objects that power up effects are applied to
{
	for (int i = 0; i < m_powerUps.size(); i++) { // for each current power up on the screen

		if (m_powerUps[i].collision(ball->getShapeReference()->getGlobalBounds())) { /// if there is a collsion between a power up and the ball
			
			if (m_powerUps[i].hasBall()) { // if the power up has an effect that will be applied to the ball
				searchForExistingBallEffect(m_powerUps[i]); // check if the effect of the power up is already active
			}
			else {
				/// give the power up a refernce to the paddle it may need to apply the effect to based on the velocity of the ball
				ball->getVelocity().x < 0.0f ? m_powerUps[i].setPaddle(AI,-1) : m_powerUps[i].setPaddle(player,1);
				searchForExistingPaddleEffect(m_powerUps[i]); // check if the effect of the power up already exists
			}
			m_powerUps.erase(m_powerUps.begin() + i); // erase the power up from the vector being used to draw them to the screen 
		}

	}


}



void powerUpsManager::searchForExistingBallEffect(powerUp &newPowerUp) {
	

	for (int i = 0; i < m_activePowerUp.size(); i++) { // for each active power up
	  if (m_activePowerUp[i].getId() == newPowerUp.getId()) { // if there is a ball power up currenlty active with the same effect 
		  
		  m_activePowerUp[i].incrementDuration(); // increment the duration of that current active power up 
		  return; // return as we dont need to push the new power up to the active power ups vector
	  } 
	}
	//otheriws the power up we were checking becomes a new active power up with an effect duration 
	newPowerUp.setTimeOfCollision(m_CollisionTimer.getElapsedTime());
	m_activePowerUp.push_back(newPowerUp);

}

// search for exsitig paddle effects 

void powerUpsManager::searchForExistingPaddleEffect(powerUp& newPowerUp) {

	for (int i = 0; i < m_activePowerUp.size(); i++) { // for each power up
		// if the effect direction of the power up we are checking is the same as another already in the active power ups vector
		if (m_activePowerUp[i].getEffectDirection() == newPowerUp.getEffectDirection()) { 
			std::cout << m_activePowerUp[i].getEffectDirection() << std::endl;
			m_activePowerUp[i].incrementDuration(); // we increment the duration of the current active power up of the same type
			return; // we return as we found a power up of the same type 
		 }

	}
	// otherwise we push the power up to the active power ups vector setting its effect time before doing so 
	// as we didnt  find an active power up with the same effect
	newPowerUp.setTimeOfCollision(m_CollisionTimer.getElapsedTime());
	m_activePowerUp.push_back(newPowerUp);


}



void powerUpsManager::manageDurationEffects(Ball*ball,float fDt) { // used to manage any duration effects power ups may have
	
	for (int i = 0; i < m_activePowerUp.size(); i++) { // loop through the active power ups vector

		m_activePowerUp[i].applyEffect(ball,fDt); // apply the power up affect to objects

		if (m_activePowerUp[i].negateEffect(ball)) { // if the power up has fully neagted its effect

			m_activePowerUp.erase(m_activePowerUp.begin() + i);// remove the power up from the active power ups vector
			
		}

	}



}
void powerUpsManager::resetTimers() { // reset powerUpManager timers
	m_eventTimer.restart();
	m_CollisionTimer.restart();
	m_currentEventTime = m_currentEventTime.Zero;
}

// draw all power ups currenlytin the m_powerups vector to the screen
void powerUpsManager::draw(sf::RenderWindow&window) {
	for (int i = 0; i < m_powerUps.size(); i++) {
		m_powerUps[i].drawPowerUp(window);
	}
}

void powerUpsManager::update(float fDt) {

	for (int i = 0; i < m_powerUps.size(); i++) { // loop through the power ups vector

		if (m_powerUps[i].getTargetHitCount() >= m_powerUps[i].getMaxTargetHitCount()) { // if the power up has reached its maximum movement target
			m_powerUps.erase(m_powerUps.begin() + i); /// erase the power up from the current power ups vector
			continue;// move to next itteration
		}
		m_powerUps[i].updatePowerUpPos(fDt); // other wise update the power up

	}

}

void powerUpsManager::generatePowerUp() {

	m_currentEventTime = m_eventTimer.getElapsedTime(); // set the current event timer to the total elapsded time of the sf::clock object used to manage when power ups should spawn
	
	if (m_currentEventTime > m_powerUpGenTime) { // if the current event timer is greater than the end time 

		m_eventTimer.restart(); // restart event clock

		// zero out all other sf::Time objects that are associated with events
		m_currentEventTime = m_currentEventTime.Zero;

		powerUp newPowerUp = powerUp(m_windowWidth, m_windowHeight); 
		// set the type of the power up based on its id in relation to the various durations and colours 
		//stored in the m_effectDurations  array and m_powerUpColours array located in the powerUpsManagerClass
		newPowerUp.setType(m_powerUpColours[newPowerUp.getId()], m_effectDurations[newPowerUp.getId()]);
		newPowerUp.setSpawn(); // set the spawn location of the power up
		m_powerUps.push_back(newPowerUp); // push the power up to the current powerups vector allowing it to be drawn and become active


	}
	



}
void powerUpsManager::clearPowerUps(Ball*ball) { // clear all power ups on the screen and active power ups

	for (int i = 0; i < m_activePowerUp.size(); i++) {
		m_activePowerUp[i].setTotalEffectTime(sf::Time::Zero);
		m_activePowerUp[i].negateEffect(ball);
	}
	m_powerUps.clear(); 
	m_activePowerUp.clear();
	
}


