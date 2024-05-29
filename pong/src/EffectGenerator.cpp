#include "effectGenerator.h"
#include <iostream>

EffectGenerator::EffectGenerator(unsigned int width, unsigned int height) { // effect generator constructor that takes in the current window width and height 
	m_windowWidth = width; // assign the passed in width and height arguments to the windowHeight and windowWidth attributes of the effectGenerator  class
	m_windowHeight = height;
	
}


void EffectGenerator::clearParticle() { // this method is used to remove all elements from the currentParticles vector, this prevents scenarios where particles that were genertaed during different game states carry over into the next state(particles generated during gameplay dont appear on the game over screen)
	currentParticles.clear();
	verticies.clear();
}

bool EffectGenerator::handleParticleCollisions(sf::FloatRect bounds) {
	
	
	for (int i = 0; i < particlesForcollision.size(); i++) {

		if (particlesForcollision[i]->getAlpha() <= 1.0f) {
			std::cout << "erase" << std::endl;
			particlesForcollision.erase(particlesForcollision.begin() + i);
			
			continue;
		}

		if (particlesForcollision[i]->getBounds().intersects(bounds)) {
			std::cout << "erase" << std::endl;
			particlesForcollision.erase(particlesForcollision.begin() + i);
			return true;
		}
	}

	return false;


}
void EffectGenerator::generateConstellationExplosion() {
	
	
	if (currentEvenetTimer - m_constellationGenerationTime >= sf::seconds(3.0f)) { // if the differnce between the currentEventTimer and our constellation genertaion timer is 3 seconds 
		// set the consetllation time to the current ensuring that the differnce is nullified 
		m_constellationGenerationTime = currentEvenetTimer; 
		//generating random floating point number for the radius of the particles with and upper bound of 10.0f and lower bound of 6.0f
		float fParticleRadius = 5.0f + static_cast<float>(rand()) / static_cast<float>((RAND_MAX / ( 8.0f - 5.0f))); 
		int irandomAmount = (rand() % (8 - 6)) + 6; // genrtaing random amount with an upper bound of 10 and lower bound of 8
		//generate particles using the radius,postion,and random amount along with allowing the particles alpha to be manipluated and giving each particle a speed range 
		generateParticles(irandomAmount, fParticleRadius, true, -75, 75);// generate particles with a speed range and allow for their alpha value to be manipulated 
	
	}
		// this loop only needs to happen during the generateConstellationExplosion method itself
		for (int i = eventStart; i < currentParticles.size(); i++) {  // from the starting index of the event(the index of the first particle that was generated above using the generate particle method) 
			
			for (int i = 0; i < verticies.size(); i++) { // for each vertex
				if (currentParticles[i].m_event == m_newEvent) {
					verticies[i].position = currentParticles[i].getShape().getPosition(); // update the postion of the vertex to be in the centre of the particle 
					verticies[i].color = currentParticles[i].getColour(); // set the colour to match the particle(this also makes the vertex fade with an alpha value

				 }
			}
		}
	
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
	currentEvenetTimer = eventTimer.getElapsedTime(); // get the current elapsed time of the event clock

	if (currentEvenetTimer > m_eventEndTime) { // if the current event timer is greater than the end time 
		
		eventTimer.restart(); // restart event clock
		hasEvent = false; // set the boolean flag for an event existing to false
		// zero out all other sf::Time objects that are associated with events
		currentEvenetTimer =  currentEvenetTimer.Zero;
		m_constellationGenerationTime =  m_constellationGenerationTime.Zero;
		m_newEvent = END; // set the m_newEvent enum to END 
		verticies.clear();
		
	}
	if (currentEvenetTimer > m_eventInitialiseTime) { // generate new event if we have reach our initialisation time
		m_newEvent = CONSTELLATION;


	}
	


}
// will be used to generate a number of particles at random postions with a random colour 
void EffectGenerator::generateParticles(int newCount, float radius,bool hasAlpha,int speedMin,int speedMax ) {

	// generate random colour range between 255 and 100 ensuring that the particles will be visible even on the lower end of the boundry
	sf::Color randomColour(
		(rand() % (255 - 100)) + 100, 
		(rand() % (255 - 100)) + 100, 
		(rand() % (255 - 100)) + 100
	);
	float frandX = (rand() % int(m_windowWidth - radius * 2)); // generate random postion based on screen width,height and radius argument 
	float frandY = (rand() % int(m_windowHeight - radius * 2));
	
	const int count = newCount;
	// create a new vertex asscoiated with each particle
	sf::Vertex newVertex(sf::Vector2f(frandX, frandY));
	newVertex.color = randomColour;
	sf::VertexArray lineVerticies = {};
	
	lineVerticies.resize(newCount);
	
	
	
	for (int i = 0; i < newCount; i++) { // generate number of particles according to newCount arg 
		// create particle object with a random postion and colour, along with the defined radius( these particles also have a modifiable alpha value) 
		Particle newParticles = Particle(frandX, frandY, randomColour, radius, hasAlpha, speedMin, speedMax);
		newParticles.m_event = m_newEvent; // the event attribute willl determine how the particle is manipluated on screen

		currentParticles.push_back(newParticles);// push particle objects to the currentParticles vector attribute of the effect generator class 
		verticies.push_back(newVertex); // push vertex objects to vertciy vector
		particlesForcollision.push_back(&currentParticles.back());
	}
	if (!hasEvent) {
		hasEvent = true;
		// get the current starting postion of the event based on the index of the first particle that was pushed to the vector above 
		eventStart = currentParticles.size() - newCount; 
	
	}
	
}

// used to generate a burst of particles each time the ball collides with the paddle 
void EffectGenerator::generateCollsionParticles(sf::Vector2f position, int direction) {  // takes in the postion of the collsion and the direction the particles need to fly according to the paddle they hit 

	int iradiusUpperBound = 4; // setting an upper bound for generating a random radius 
	int iradiusLowerBound = 1; /// setting lower bound 
	
	
	for (int i = 0; i < 5; i++) { // generate 5 particles on each collsion

		float fradius = (rand() % (iradiusUpperBound - iradiusLowerBound) + iradiusLowerBound); // egenrate random radius for the particle depeding on upper and lower bound values 
		Particle newParticle = Particle(position.x, position.y, sf::Color::White, fradius, true, -100, 100); // gernate a new particle object to be pushed to the currentParticle vector arrtibute of the effect geenrator class allowing the particles to be drawn onto the screen and updated 
		// these particles have a speed range of -100 to 100
		sf::Vector2f newParicleVelocity(abs(newParticle.getSpeed()) * direction, newParticle.getSpeed());
		
		// the above vector will determine the direction the particles will go in on the x and y  in this case the particles will always travel in the opposite direction of the paddle they just hit 
		// particles will travel up and down on y ( negative or positive) depending on  their generated speed value (-100 to 100)
		newParticle.setVelocity(newParicleVelocity);
		
		currentParticles.push_back(newParticle); // push the new particle object to the currentParticles vector 

	}




}


void EffectGenerator::drawVerticies(sf::RenderWindow& window) {
	
	if (!verticies.empty()) {
		window.draw(verticies.data(), verticies.size(), sf::LineStrip);
	}
	

}

void EffectGenerator::drawShapes(sf::RenderWindow& window) { // this is the main method that will be called by an effect generator object defined in the game engine header file/cpp and is used to draw each particle object currently active on the screen 
	for (int i = 0; i < currentParticles.size(); i++) { // for each particle object in the vector 'currentParticles'
		currentParticles[i].draw(window); // call draw method for each particle object
        	
	}
	
}
void EffectGenerator::updateVertcies() {
	for (int i = 0; i < verticies.size(); i++) {
		if (verticies[i].color.a <= 1.0f) { // if the vertex alpha is smaller or equal to 1
			verticies.erase(verticies.begin() + i); // erase the vertex from the vector
		}
	}
	
}
void EffectGenerator::update(float dt) { // the effect generator is used to manage all of the current particles on screen therefore it only needs to update the particles and check for condtions where they would need to disappear 

	for (int i = 0; i < currentParticles.size(); i++) { // loop through all of the current particle objects contained wihtin the vector attribute of the effect geenrator class 

		if (currentParticles[i].getAlpha() <= 0.0f) { // if the currentPaticle being checked in the vector has an alpha value smaller than or equal to 0 meaning it is almost no longer visible

			currentParticles.erase(currentParticles.begin() + i); // remove the particle object from the vector so it is no longer drawn/updated erase also automalically adjusts the size of the vector based on the number of elements removed 
			
			
			
		}
		else { // otherwise if there ar no condtions that would require the particle t be removed from the vector 

			currentParticles[i].update(dt); // for each particle update its postion and any other attributes of the particle object such as an alpha value for its colour 

		}
	}


}