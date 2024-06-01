#include "PowerUpsManager.h"
#include <iostream>


powerUpsManager::powerUpsManager(float windowWidth,float windowHeight) {

	m_powerUpGenTime = sf::seconds(2.0f); // time it takes to spawn new power ups 
	m_windowWidth = windowWidth; /// window widht and height vairbales used for spawning power ups
	m_windowHeight = windowHeight;
	
}


void powerUpsManager::handleCollision(Ball*ball,Paddle*player,Paddle*AI) // pass refernce to objects that power up effects are applied to
{
	for (int i = 0; i < m_powerUps.size(); i++) { // for each current power up on the screen

		if (m_powerUps[i].collision(ball->getShapeReference()->getGlobalBounds())) { /// if there is a collsion between a power up and the ball
			if (!(searchForExistingEffect(m_powerUps[i]))) { // if the powerups current effect cannot be found in the active power upps vector 
		      
			 m_powerUps[i].setTimeOfCollision(m_CollisionTimer.getElapsedTime()); // set the colsion time of the power up to the current elapsed time of the m_collsion time object
			 ball->getVelocity().x < 0.0f ? m_powerUps[i].setPaddle(AI):m_powerUps[i].setPaddle(player); /// give the power up a refernce to the paddle it may need to apply the effect to based on the velocity of the ball
			 m_activePowerUp.push_back(m_powerUps[i]); // push the power up to the active power up vector

			}
			m_powerUps.erase(m_powerUps.begin() + i); // erase the power up from the vector being used to draw them to the screen 
		}

	}


}

bool powerUpsManager::searchForExistingEffect(powerUp &newPowerUp) {

	for (int i = 0; i < m_activePowerUp.size(); i++) { // for each active power up
	  if (m_activePowerUp[i].getId() == newPowerUp.getId()) { // if there is a power up currenlty active with the same effect 
		
		  m_activePowerUp[i].incrementDuration(newPowerUp.getDuration()); // increment the duration of that current active power up using the duration of the power up who's effect we were searching for
		  std::cout << m_activePowerUp[i].getDuration().asSeconds() << std::endl;
	      return true; // return true as we found an effect that matched the power up effect we were searching for
	 } 
	}
	return false; // other wise return false and the power up who's effect id we were searching for gets pushed to the active power ups vector and becomes an active power up


}
void powerUpsManager::manageDurationEffects(Ball*ball,float dt) { // used to manage any duration effects power ups may have
	
	for (int i = 0; i < m_activePowerUp.size(); i++) { // loop through the active power ups vector

		m_activePowerUp[i].applyEffect(ball,dt); // apply the power up affect to objects

		if (m_activePowerUp[i].negateEffect(ball,dt)) { // if the power up has fully neagted its effect

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

void powerUpsManager::update(float dt) {

	for (int i = 0; i < m_powerUps.size(); i++) { // loop through the power ups vector

		if (m_powerUps[i].getTargetHitCount() >= m_powerUps[i].getMaxTargetHitCount()) { // if the power up has reached its maximum movement target
			m_powerUps.erase(m_powerUps.begin() + i); /// erase the power up from the current power ups vector
			continue;// move to next itteration
		}
		m_powerUps[i].updatePowerUpPos(dt); // other wise update the power up

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
void powerUpsManager::clearPowerUps() { // clear all power ups on the screen and active power ups
	m_powerUps.clear(); 
	m_activePowerUp.clear();
	
}


