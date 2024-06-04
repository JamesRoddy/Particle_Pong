#pragma once
#include "effectGenerator.h"
#include <iostream>
EffectGenerator::EffectGenerator(float fwindowwidth, float fwindowHeight) { // effect generator constructor that takes in the current window width and height 
	
	m_windowWidth = fwindowwidth; // assign the passed in width and height arguments to the windowHeight and windowWidth attributes of the effectGenerator  class
	m_windowHeight = fwindowHeight;
	m_eventEndTime = sf::seconds(30); // determine amount of time it takes for each event to end
	m_eventInitialiseTime = sf::seconds(15); // how long it takes for an event to fire 
	m_hasEvent = false; // boolean flag for having an event that is used to control the postion of the start index
	m_hasStartIndex = false;
	
	m_eventStartIndex = 0; // set initial starting index of the event to 0
	m_explosionAmount = 6;
	m_eventTextOffset = 75; // set the offset for the event text allowing for control over where it is placed in fX or fY
	m_eventTextSize = 18;
	m_warningShouldScale = true;
	m_warningTextScalar = 1.5f;
	m_eventWarningSign.setSize(sf::Vector2f(15.0f, 15.0f));
	
	m_warningSignTexture.loadFromFile(".\\assets\\textures\\eventWarning.jpg");
	m_eventWarningSign.setTexture(&m_warningSignTexture);
	m_eventWarningSign.setScale(sf::Vector2f(0.0f, 0.0f));
	m_eventWarningSign.setOrigin(m_eventWarningSign.getSize().x / 2, m_eventWarningSign.getSize().y / 2);
	m_eventTextFont.loadFromFile(".\\assets\\fonts\\digital-7.ttf"); // loading the font that will be applied to the event time display
	m_eventText.setFont(m_eventTextFont); //set the font of the m_eventText object
	m_eventText.setFillColor(sf::Color::White); // set the fill colour
	m_eventText.setCharacterSize(m_eventTextSize); 
	
	m_eventText.setPosition((m_windowWidth / 2 - m_eventTextSize*4), m_eventTextOffset); // adjust the text postion so that it is centred and at the top of the screen
	m_eventWarningSign.setPosition(m_windowWidth/2, (m_eventText.getPosition().y+m_eventTextSize) + m_eventWarningSign.getSize().y);
	m_newEvent = END; // used to control what event should fire
	
}


void EffectGenerator::clearParticle() { // this method is used to remove all elements from the m_currentParticles vector, this prevents scenarios where particles that were genertaed during different game states carry over into the next state(particles generated during gameplay dont appear on the game over screen)
	m_currentParticles.clear();

}



void EffectGenerator::handleParticleCollisions(Ball *ball) { // used to handle collisions between particles
	
	for (int i = 0; i < m_currentParticles.size(); i++) { 
		
		if (m_currentParticles[i].hasCollided(ball) && m_currentParticles[i].getCollision()) { // if the particle intersects with the passed in bounds and it has an event attached to it
			
			if (!ball->getVelocity().x <= ball->getDefaultSpeed()) {
				ball->setVelocity(-ball->getVelocity() * m_currentParticles[i].getSpeedDecrease());
			}
			
			m_currentParticles.erase(m_currentParticles.begin() + i); // erase the particle
		}
	 }

}




void EffectGenerator::generateExplosion() {
	
	
	if (m_currentEvenetTimer - m_explosionGenerationTime >= sf::seconds(2.5f)) { // if the differnce between the m_currentEventTimer and our constellation genertaion timer is 3 seconds 
		// set the explosion time to the currentEvenetTimer ensuring that the differnce is nullified 
		m_explosionGenerationTime = m_currentEvenetTimer; 
		//generating random floating point number for the fRadius of the particles with and upper bound of 8.0f and lower bound of 6.0f
		float fParticleRandomRadius = generateRandomValue(8.0f,6.0f); 
		int iRandomAmount = generateRandomValue(8,6); // generating random amount with an upper bound of 8 and lower bound of 6
		float fPostionX = generateRandomValue(m_windowWidth, m_windowWidth / 4);
		float fPositionY = generateRandomValue(m_windowHeight, m_windowHeight / 4);
		for (int i = 0; i < m_explosionAmount; i++) {
			// generate random initial postion for the explosions withn the bounds of the screen
			//generate particles with a speed range and allow for their alpha value to be manipulated
			 generateParticles(iRandomAmount, fParticleRandomRadius, true, -400, 400, fPostionX, fPositionY,true);
			  

		}
	   
	}
		
	
}

void EffectGenerator:: manageEvents() {
	// manage events that fire based on the value of the m_newEvent attribute that is set
	// to an enum constant contained within the Event enum type
	switch (m_newEvent) {
	 case PARTICLESTORM:
		generateExplosion();
		break;
	 
	}


}

void EffectGenerator::generateEvent(float fDt) {
	// control the rate at which different event effects will fire 
	m_currentEvenetTimer = m_eventTimer.getElapsedTime(); // get the current elapsed time of the event clock
	m_displayTime = m_eventInitialiseTime - m_currentEvenetTimer; // set the initial display time to the differnece between our eventInitialise time and our current event time 

	if (m_currentEvenetTimer > m_eventEndTime) { // if the current event timer is greater than the end time 
		
		m_eventTimer.restart(); // restart event clock
		m_hasStartIndex = false; // set the boollean flag for having a starting index for the event to false meaning that when a new event is generated we only loop through particles that we need to in terms of their index
		m_hasEvent = false; // set the boolean flag for an event existing to false
		// zero out all other sf::Time objects that are associated with events
		
		m_currentEvenetTimer =  m_currentEvenetTimer.Zero;
		m_explosionGenerationTime =  m_explosionGenerationTime.Zero;
		
		m_newEvent = END; // set the m_newEvent enum to END 
		
		
	}
	if (m_currentEvenetTimer > m_eventInitialiseTime) { // generate new event if we have reach our initialisation time
		
		m_hasEvent = true; // set the flag for having an event to true
		m_newEvent = PARTICLESTORM; // set the event enum to the current event


	}
	setEventDisplayText(fDt);
	


}

void EffectGenerator::updateEventWarnings(float fDt) {

	sf::Vector2f currentScale = m_eventWarningSign.getScale();//. current scale of warning sign 
	std::cout << currentScale.x << std::endl;
	if (m_warningShouldScale && currentScale.x <=1.0f) { // if the warning sign should scale and 
		std::cout << "should scale" << std::endl;
		m_eventWarningSign.setScale(currentScale.x + fDt * m_warningTextScalar, currentScale.y + fDt * m_warningTextScalar); // increase scale of warning sign
		return; // return out of the function as we dont need to descale the warning sign if it hasnt reached its full scale

	}
	std::cout << "shouldnt scale" << std::endl;

	m_warningShouldScale = false; // if we have reached a scale where the warning sign is at full size
	m_eventWarningSign.setScale(currentScale.x - fDt * m_warningTextScalar, currentScale.y - fDt * m_warningTextScalar);// scale the object down
	
	m_eventWarningSign.getScale().x <= 0.0f ? m_warningShouldScale = true : m_warningShouldScale = false;// check for when the scale hits zero and set should scale to true if it does
	

	


}


void EffectGenerator::resetEventTimer() { // used to reset the event timer and event hud when switching game states
	m_eventTimer.restart();// restart event clock
	// zero out timers associated with events
	m_currentEvenetTimer = m_currentEvenetTimer.Zero;
	m_displayTime = m_displayTime.Zero;
	// set bools for controlling events to false
	m_hasStartIndex = false;
	m_hasEvent = false;
	m_eventText.setString("");// set event text to blanck
	m_eventWarningSign.setScale(sf::Vector2f(0.0f, 0.0f));// set warning sign scale to 0

}
// used to update the m_eventText object that displays how long until an event will fire
void EffectGenerator:: setEventDisplayText(float fDt) {
	std::stringstream ssEventDisplayText; // create temporary local string stream variable that willl be refereshed each time this function is called
	if (!m_hasEvent) {
		ssEventDisplayText << "particle storm in " << m_displayTime.asSeconds() << "\n"; // if we dont have an event displaye the current time until the event
		if (m_displayTime.asSeconds() <= 5.0f) updateEventWarnings(fDt);
		
	}
	else {

		m_eventWarningSign.setScale(sf::Vector2f(0.0f, 0.0f));

	}
	
	m_eventText.setString(ssEventDisplayText.str()); // Update the current string assigned to the m_eventText object with the new event time

}
//draw the event text to the window to be rendered 
void EffectGenerator::drawEventText(sf::RenderWindow &window) {
	window.draw(m_eventText);
	window.draw(m_eventWarningSign);
}
 
// will be used to generate a number of particles at random postions with a random colour 
void EffectGenerator::generateParticles(int iNewcount, float fRadius,bool bHasAlpha,int iSpeedMin,int iSpeedMax,float fX, float fY,bool bHasCollsion ) {

	// generate random colour range between 255 and 100 ensuring that the particles will be visible even on the lower end of the boundry
	sf::Color randomColour(
		(rand() % (255 - 100)) + 100, 
		(rand() % (255 - 100)) + 100, 
		(rand() % (255 - 100)) + 100
	);
	
	
	for (int i = 0; i < iNewcount; i++) { // generate number of particles according to iNewcount arg 
		// create particle object with a random postion and colour, along with the defined fRadius( these particles also have a modifiable alpha value) 
		Particle newParticles = Particle(fX, fY, randomColour, fRadius, bHasAlpha, iSpeedMin, iSpeedMax,bHasCollsion);
		newParticles.m_event = m_newEvent; // the event attribute willl determine how the particle is manipulated on screen
		
		m_currentParticles.push_back(newParticles);// push particle objects to the m_currentParticles vector attribute of the effect generator class 
	}
	if (!m_hasStartIndex) { // if we are generating 
		m_hasStartIndex = true;
		// get the current starting postion of the event based on the index of the first particle that was pushed to the vector above 
		m_eventStartIndex = m_currentParticles.size() - iNewcount; 
	
	}
	
}
// used to generate a burst of particles each time the ball collides with the paddle 
void EffectGenerator::generateCollsionParticles(sf::Vector2f fPosition, int iDirection,sf::Color objectColour) {  // takes in the postion of the collsion and the direction the particles need to fly according to the paddle they hit 

	float fRadiusUpperBound = 4; // setting an upper bound for generating a random fRadius 
	float fRadiusLowerBound = 1; /// setting lower bound 
	
	
	for (int i = 0; i < 5; i++) { // generate 5 particles on each collsion

		float fradius = generateRandomValue(fRadiusUpperBound,fRadiusLowerBound); // egenrate random fRadius for the particle depeding on upper and lower bound values 
		Particle newParticle = Particle(fPosition.x, fPosition.y, objectColour, fradius, true, -100, 100,false); // gernate a new particle object to be pushed to the currentParticle vector arrtibute of the effect geenrator class allowing the particles to be drawn onto the screen and updated 
		// these particles have a speed range of -100 to 100
		sf::Vector2f newParicleVelocity(abs(newParticle.getSpeed()) * iDirection, newParticle.getSpeed());
		
		// the above vector will determine the direction the particles will go in on the fX and fY  in this case the particles will always travel in the opposite direction of the paddle they just hit 
		// particles will travel up and down on fY ( negative or positive) depending on  their generated speed value (-100 to 100)
		newParticle.setVelocity(newParicleVelocity);
		
		m_currentParticles.push_back(newParticle); // push the new particle object to the m_currentParticles vector 

	}

}

void EffectGenerator::drawShapes(sf::RenderWindow& window) { // this is the main method that will be called by an effect generator object defined in the game engine header file/cpp and is used to draw each particle object currently active on the screen 
	
	for (int i = 0; i < m_currentParticles.size(); i++) { // for each particle object in the vector 'm_currentParticles'
		m_currentParticles[i].draw(window); // call draw method for each particle object
        	
	}
	
}

float EffectGenerator::generateRandomValue(float fUpperBound, float fLowerBound) {

	return fLowerBound + static_cast<float>(rand()) / static_cast<float>((RAND_MAX / (fUpperBound - fLowerBound)));

}

int EffectGenerator::generateRandomValue(int iUpperBound, int iLowerBound) {
	return (rand() % (iUpperBound - iLowerBound) + iLowerBound);
}

void EffectGenerator::update(float fDt) { // the effect generator is used to manage all of the current particles on screen therefore it only needs to update the particles and check for condtions where they would need to disappear 

	for (int i = 0; i < m_currentParticles.size(); i++) { // loop through all of the current particle objects contained wihtin the vector attribute of the effect geenrator class 

		if (m_currentParticles[i].getAlpha() <= 0.0f) { // if the currentPaticle being checked in the vector has an alpha value smaller than or equal to 0 meaning it is almost no longer visible
			m_currentParticles.erase(m_currentParticles.begin() + i); // remove the particle object from the vector so it is no longer drawn/updated erase also automalically adjusts the size of the vector based on the number of elements removed 
			
		}
		else {
			m_currentParticles[i].update(fDt); // for each particle update its postion and any other attributes of the particle object such as an alpha value for its colour 

		}

	}

}