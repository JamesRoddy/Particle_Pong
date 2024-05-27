#include "effectGenerator.h"
#include <iostream>
EffectGenerator::EffectGenerator(unsigned int width, unsigned int height) { // effect generator constructor that takes in the current window width and height 
	m_windowWidth = width; // assign the passed in width and height arguments to the windowHeight and windowWidth attributes of the effectGenerator  class
	m_windowHeight = height;

}


void EffectGenerator::clearParticle() { // this method is used to remove all elements from the currentParticles vector, this prevents scenarios where particles that were genertaed during different game states carry over into the next state(particles generated during gameplay dont appear on the game over screen)
	currentParticles.clear();

}

void EffectGenerator::generateConstellation() {
	
    for (int i = 0; i < currentParticles.size(); i++) {

		verticies[i].color = currentParticles[i].getColour();
		verticies[i].position = currentParticles[i].getShape().getPosition();
	

	}

}



// will be used to generate a number of particles at random postions with a random colour 
void EffectGenerator::generateParticles(int newCount, float radius, sf::Vector2f position) {

	
	float fnewColorValue = rand() % 255; // generate random colour value up to 255
	float frandX = (rand() % int(m_windowWidth - radius * 2)); // generate random postion based on screen widht  height and radius argument 
	float frandY = (rand() % int(m_windowHeight - radius * 2));

	for (int i = 0; i < newCount; i++) { // generate number of particles according to newCount arg 

		Particle newParticle = Particle(frandX, frandY, sf::Color::Color(fnewColorValue, fnewColorValue, fnewColorValue, 255), radius, true); // create particle object with a random postion and colour, aong with the defined radius( these particles also have a modifiable alpha value) 
		currentParticles.push_back(newParticle);// push particle object to the currentParticles vector attribute of the effect generator class 
		verticies.push_back( sf::Vertex(newParticle.getShape().getPosition()));
		// this will allow the particles to be drawn and updated 
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
		// particles will travel up and down on y ( n or p) depedning on  their generated speed value (-100 to 100)
		newParticle.setVelocity(newParicleVelocity);
		verticies.push_back(sf::Vertex(newParticle.getShape().getPosition()));

		currentParticles.push_back(newParticle); // push the new particle object to the currentParticles vector 

	}




}


void EffectGenerator::drawVerticies(sf::RenderWindow& window) {
	
	window.draw(verticies.data(),verticies.size( ),sf::Lines);

}

void EffectGenerator::drawShapes(sf::RenderWindow& window) { // this is the main method that will be called by an effect generator object defined in the game engine header file/cpp and is used to draw each particle object currently active on the screen 
	for (int i = 0; i < currentParticles.size(); i++) { // for each particle object in the vector 'currentParticles'
		currentParticles[i].draw(window); // call draw method for each particle object
        	
	}
}

void EffectGenerator::update(float dt) { // the effect generator is used to manage all of the current particles on screen therefore it only needs to update the particles and check for condtions where they would need to disappear 

	for (int i = 0; i < currentParticles.size(); i++) { // loop through all of the current particle objects contained wihtin the vector attribute of the effect geenrator class 

		if (currentParticles[i].getAlpha() <= 0.0f) { // if the currentPaticle being checked in the vector has an alpha value smaller than or equal to 0 meaning it is no longer visible

			currentParticles.erase(currentParticles.begin() + i); // remove the particle object from the array so it is no longer drawn/updated erase also automalically adjusts the size of the vector based on the number of elements removed 
			verticies.erase(verticies.begin() + i);
		}
		else { // otherwise if there ar no condtions that would require the particle t be removed from the vector 

			currentParticles[i].update(dt); // for each particle update its postion and any other attributes of the particle object such as an alpha value for its colour 

		}
	}


}