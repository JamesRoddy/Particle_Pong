#include "effectGenerator.h"
#include <iostream>

EffectGenerator::EffectGenerator(unsigned int width, unsigned int height) { // effect generator constructor that takes in the current window width and height 
	m_windowWidth = width; // assign the passed in width and height arguments to the windowHeight and windowWidth attributes of the effectGenerator  class
	m_windowHeight = height;
	m_eventEndTime = sf::seconds(30); // determine amount of time it takes for each event to end
	m_eventInitialiseTime = sf::seconds(15); // how long it takes for an event to fire 
	m_hasEvent = false; // boolean flag for having an event that is used to control the postion of the start index
	m_newEvent = END; // used to control what event should fire
	m_eventStartIndex = 0; // set initial starting index of the event to 0
}

void EffectGenerator:: resetEventTimer() {
	m_eventTimer.restart();
    m_currentEvenetTimer = m_currentEvenetTimer.Zero;

}
void EffectGenerator::clearParticle() { // this method is used to remove all elements from the m_currentParticles vector, this prevents scenarios where particles that were genertaed during different game states carry over into the next state(particles generated during gameplay dont appear on the game over screen)
	m_currentParticles.clear();
	m_verticies.clear();
}



bool EffectGenerator::handleParticleCollisions(sf::FloatRect bounds) {
	
	for (int i = 0; i < m_currentParticles.size(); i++) {
		
		if (m_currentParticles[i].hasCollided(bounds) && m_currentParticles[i].m_event != 0) {
			m_currentParticles.erase(m_currentParticles.begin() + i);
			return true;
		}
	 }
	return false;

}



void EffectGenerator::generateConstellationExplosion() {
	
	
	if (m_currentEvenetTimer - m_constellationGenerationTime >= sf::seconds(2.0f)) { // if the differnce between the m_currentEventTimer and our constellation genertaion timer is 3 seconds 
		// set the consetllation time to the current ensuring that the differnce is nullified 
		
		m_constellationGenerationTime = m_currentEvenetTimer; 
		//generating random floating point number for the radius of the particles with and upper bound of 10.0f and lower bound of 6.0f
		float fParticleRadius = 5.0f + static_cast<float>(rand()) / static_cast<float>((RAND_MAX / ( 8.0f - 5.0f))); 
		int irandomAmount = (rand() % (10 - 8)) + 8; // genrtaing random amount with an upper bound of 10 and lower bound of 6
		int explosionAmount = 4;
		
		float fpostionX = static_cast<float>(rand() % (m_windowWidth - m_windowWidth / 4 - (0 + m_windowWidth / 4)) + (0 + m_windowWidth / 4)); // generate random initial postion for the explosions withn the bounds of the screen
		float fpositionY = static_cast<float>(rand() % (m_windowHeight - m_windowHeight/4 - (0 + m_windowHeight / 4)) + (0 + m_windowHeight / 4));
		float explosionPositionIncrementX =  fpostionX<static_cast<float> (m_windowWidth/2) ? 50.0f:-50.0f; // used to increment the random postion for the explosions each time the event triggered keeping them close but also on screen
		float explosionPositionIncrementY = fpositionY < static_cast<float>( m_windowHeight / 2) ? 50.0f : -50.0f;
		//generate particles using the radius,postion,and random amount along with allowing the particles alpha to be manipluated and giving each particle a speed range 
		
	
		for (int i = 0; i < explosionAmount; i++) {

			generateParticles(irandomAmount, fParticleRadius, true, -400, 400, fpostionX, fpositionY);// generate particles with a speed range and allow for their alpha value to be manipulated 
			fpostionX += explosionPositionIncrementX; // move the random postion by a certain the explosion increment ensuring that all the explosions are geenrated on screen and at vaired position
			fpositionY += explosionPositionIncrementX;

		}
	   
	}
		// this loop only needs to happen during the generateConstellationExplosion method itself
		//for (int i = m_eventStartIndex; i < m_currentParticles.size(); i++) {  // from the starting index of the event(the index of the first particle that was generated above using the generate particle method) 
		//	
		//	for (int i = 0; i < m_verticies.size(); i++) { // for each vertex
		//		if (m_currentParticles[i].m_event == m_newEvent) { // if the particle we are targeting has the correct event
		//			m_verticies[i].position = m_currentParticles[i].getShape().getPosition(); // update the postion of the vertex to be in the centre of the particle 
		//			m_verticies[i].color = m_currentParticles[i].getColour(); // set the colour to match the particle(this also makes the vertex fade with an alpha value

		//		}
		//	}
		//}
	
}


void EffectGenerator:: manageEvents(sf::Vector2f position, sf::Vector2f ballVelocity) {
	// manage events that fire based on the value of the m_newEvent attribute that is set
	// to an enum constant contained within the Event enum type
	switch (m_newEvent) {
	 case CONSTELLATION:
		generateConstellationExplosion();
		break;
	 
	}


}

void EffectGenerator::generateEvent() {
	// control the rate at which different event effects will fire 
	m_currentEvenetTimer = m_eventTimer.getElapsedTime(); // get the current elapsed time of the event clock

	if (m_currentEvenetTimer > m_eventEndTime) { // if the current event timer is greater than the end time 
		
		m_eventTimer.restart(); // restart event clock
		m_hasEvent = false; // set the boolean flag for an event existing to false
		// zero out all other sf::Time objects that are associated with events
		m_currentEvenetTimer =  m_currentEvenetTimer.Zero;
		m_constellationGenerationTime =  m_constellationGenerationTime.Zero;
		m_newEvent = END; // set the m_newEvent enum to END 
		m_verticies.clear(); //clear any vertcies that were associated with the event
		
	}
	if (m_currentEvenetTimer > m_eventInitialiseTime) { // generate new event if we have reach our initialisation time
		m_newEvent = CONSTELLATION;


	}
	


}
// will be used to generate a number of particles at random postions with a random colour 
void EffectGenerator::generateParticles(int newCount, float radius,bool hasAlpha,int speedMin,int speedMax,float x, float y ) {

	// generate random colour range between 255 and 100 ensuring that the particles will be visible even on the lower end of the boundry
	sf::Color randomColour(
		(rand() % (255 - 100)) + 100, 
		(rand() % (255 - 100)) + 100, 
		(rand() % (255 - 100)) + 100
	);
	
	// create a new vertex asscoiated with each particle
	
	/*sf::Vertex newVertex(sf::Vector2f(frandX, frandY));
	newVertex.color = randomColour;*/
	for (int i = 0; i < newCount; i++) { // generate number of particles according to newCount arg 
		// create particle object with a random postion and colour, along with the defined radius( these particles also have a modifiable alpha value) 
		Particle newParticles = Particle(x, y, randomColour, radius, hasAlpha, speedMin, speedMax, true);
		newParticles.m_event = m_newEvent; // the event attribute willl determine how the particle is manipulated on screen
		
		//m_verticies.push_back(newVertex); // push vertex objects to verticy vector
		m_currentParticles.push_back(newParticles);// push particle objects to the m_currentParticles vector attribute of the effect generator class 
		m_particlesForcollision.push_back(&m_currentParticles.back()); // pass a refernce to the newly stored object to the vector for handling collsion
	}
	if (!m_hasEvent) {
		m_hasEvent = true;
		// get the current starting postion of the event based on the index of the first particle that was pushed to the vector above 
		m_eventStartIndex = m_currentParticles.size() - newCount; 
	
	}
	
}

// used to generate a burst of particles each time the ball collides with the paddle 
void EffectGenerator::generateCollsionParticles(sf::Vector2f position, int direction) {  // takes in the postion of the collsion and the direction the particles need to fly according to the paddle they hit 

	int iradiusUpperBound = 4; // setting an upper bound for generating a random radius 
	int iradiusLowerBound = 1; /// setting lower bound 
	
	
	for (int i = 0; i < 5; i++) { // generate 5 particles on each collsion

		float fradius = (rand() % (iradiusUpperBound - iradiusLowerBound) + iradiusLowerBound); // egenrate random radius for the particle depeding on upper and lower bound values 
		Particle newParticle = Particle(position.x, position.y, sf::Color::White, fradius, true, -100, 100,false); // gernate a new particle object to be pushed to the currentParticle vector arrtibute of the effect geenrator class allowing the particles to be drawn onto the screen and updated 
		// these particles have a speed range of -100 to 100
		sf::Vector2f newParicleVelocity(abs(newParticle.getSpeed()) * direction, newParticle.getSpeed());
		
		// the above vector will determine the direction the particles will go in on the x and y  in this case the particles will always travel in the opposite direction of the paddle they just hit 
		// particles will travel up and down on y ( negative or positive) depending on  their generated speed value (-100 to 100)
		newParticle.setVelocity(newParicleVelocity);
		
		m_currentParticles.push_back(newParticle); // push the new particle object to the m_currentParticles vector 

	}

}


void EffectGenerator::drawVerticies(sf::RenderWindow& window) {
	
	if (!m_verticies.empty()) {

		window.draw(m_verticies.data(), m_verticies.size(), sf::LineStrip);
	}
	

}

void EffectGenerator::drawShapes(sf::RenderWindow& window) { // this is the main method that will be called by an effect generator object defined in the game engine header file/cpp and is used to draw each particle object currently active on the screen 
	
	for (int i = 0; i < m_currentParticles.size(); i++) { // for each particle object in the vector 'm_currentParticles'
		m_currentParticles[i].draw(window); // call draw method for each particle object
        	
	}
	
}
void EffectGenerator::updateVertcies() {
	for (int i = 0; i < m_verticies.size(); i++) {
		if (m_verticies[i].color.a <= 1.0f) { // if the vertex alpha is smaller or equal to 1
			m_verticies.erase(m_verticies.begin() + i); // erase the vertex from the vector
		}
	}
	
}
void EffectGenerator::update(float dt) { // the effect generator is used to manage all of the current particles on screen therefore it only needs to update the particles and check for condtions where they would need to disappear 

	for (int i = 0; i < m_currentParticles.size(); i++) { // loop through all of the current particle objects contained wihtin the vector attribute of the effect geenrator class 

		if (m_currentParticles[i].getAlpha() <= 0.0f) { // if the currentPaticle being checked in the vector has an alpha value smaller than or equal to 0 meaning it is almost no longer visible
			m_currentParticles.erase(m_currentParticles.begin() + i); // remove the particle object from the vector so it is no longer drawn/updated erase also automalically adjusts the size of the vector based on the number of elements removed 
			
		}
		else {
			m_currentParticles[i].update(dt); // for each particle update its postion and any other attributes of the particle object such as an alpha value for its colour 

		}

	}

}