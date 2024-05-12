#include "particle.h"
#include<iostream>


// a particle class that defines a circular particle with a speed range, alpha value, radius, position, and color,
particle::particle(float startX, float startY, sf::Color colour, float radius, bool hasAlpha, int speedMin , int speedMax ) {

	m_particleX = startX; // set the m_particle X and y attributes of the particle class the the startX and startY psotions passed into the constructor 
	m_particleY = startY;

	m_speed = (rand() % (speedMax - speedMin) + speedMin); // define the speed of the particle by geernating a random value bewteen the speed range min and max(including those numbers)
	m_velocity.x = m_speed; /// set the velcoity vector's x and y to the speed generated 
	m_velocity.y = m_speed; // this vector is what will be used to move each particle on the screen 

	m_hasAlpha = hasAlpha; // some particles may have their alpha values manipulated therefore this bool attribute 'm_hasAlpha' will be used to determine if the particles alpha should be changed 
	m_colour = colour;  // set the colour attribute of the particle 

	// setting the properties of the particles shape object(in this case the in built sf::circle object)
	m_particleShape.setRadius(radius); // set the radius of the particle using the radius argument passed into the constructor  
	m_particleShape.setPosition(m_particleX, m_particleY); //set pos
	m_particleShape.setFillColor(colour);// set colour 
	m_particleShape.setOrigin(m_particleShape.getRadius() / 2, m_particleShape.getRadius() / 2); // set origin based on radius 



}

void particle::move(float dt) { // method od the particle class used to update the pos of the particles shape 

	this->m_particleShape.move(m_velocity*dt); // get the particles shape attribute  and call the predefined move method 
	// moving the particle's shape  at on offset of its velocity vector attribute multiplied by dt to move it at a consistent rate 

	/*if (m_hasAlpha) {
		m_colour.a = m_alpha;
		this->m_particleShape.setFillColor(m_colour);

		if (!(m_alpha <= 0.0f)) {
			m_alpha -= 0.1f;
		}
	}*/
	

}
void particle::draw(sf::RenderWindow& window) { // the draw method for the particle that will be called in a foreach loop to draw each aprticle to the screen(this loop can be located in the effectGenerator class)

	window.draw(this->m_particleShape); // call the draw method through the refernce argument to the window to draw the particles shape(an attribute of the particle class)
	

}


effectGenerator::effectGenerator(unsigned int width, unsigned int height) { // effect egenrator constructor that takes in the current window width and height 
	windowWidth = width; // asgin the passed in widht and height arguments to the windowHeight and windowWidth attributes of the effectGenerator  class
	windowHeight = height;

}



// will be used to generate a number of particles at random postions with a random colour 
void effectGenerator::generateParticles(int newCount, float radius, sf::Vector2f position ) { 


	float newColorValue = rand() % 255; // genertae random colour value up to 255
	float randX = (rand() % int(windowWidth - radius * 2)); // generate random postion based on screen widht  height and radius argument 
	float randY = (rand() % int(windowHeight - radius * 2));
	
	for (int i = 0; i < newCount; i++) { // generate number of particles according to newCount arg 
		
		particle newParticle =  particle(randX,randY, sf::Color::White, radius, true); // create particle object with a random postion and colour, aong with the defined radius( these particles also have a modifiable alpha value) 
		currentParticles.push_back(newParticle);// push particle object to the currentParticles vector attribute of the effect generator class 
		// this will allow the particles to be drawn and updated 
	}




}
// used to generate a burst of particles each time the ball collides with the paddle 
void effectGenerator::generateCollsionParticles( sf::Vector2f position, int direction) {  // takes in the postion of the collsion and the direction the particles need to fly according to the paddle they hit 

	int iradiusUpperBound = 5; // setting an upper bound for genrtaing a random radius 
	int iradiusLowerBound = 2; /// setting lower bound 
	
	
	for (int i = 0; i < 5; i++) { // generate 5 particles on each collsion
		
		float fRadius = (rand() % (iradiusUpperBound - iradiusLowerBound) + iradiusLowerBound); // egenrate random radius for the particle depeding on upper and lower bound values 
		particle newParticle = particle(position.x, position.y, sf::Color::White, fRadius, true, -100, 100); // gernate a new particle object to be pushed to the currentParticle vector arrtibute of the effect geenrator class allowing the particles to be drawn onto the screen and updated 
		// these particles have a speed range of -100 to 100
		newParticle.m_velocity.x = abs(newParticle.m_speed) * direction; // dertmine the direction the particles will go in on the x and y with the particles in this case the particles will always tarvel in the opposite direction of the paddle they just hit 
		newParticle.m_velocity.y = newParticle.m_speed; // particles will travel up and down on y ( n or p) depedning on  their generated speed value (-100 to 100)

		currentParticles.push_back(newParticle); // push the new particle object to the currentParticles vector 

	}




}


void effectGenerator::draw(sf::RenderWindow& window) { // this is the main method that will be called by an effect geenrator object defined in the game engine header file/cpp and is used to draw each particle object currently active on the screen 
	for (particle newParticle : currentParticles) { // for each particle object in the vector 'currentParticles'
		newParticle.draw(window); // call each particle objects draw method
	}
}

void effectGenerator::update(float dt) { // the effect egenrator is used to amange all of the current particles on screen therefore it only needs to update the particles and check for condtions where they would need to disappear 
	
	for (int i = 0; i < currentParticles.size();i++) { // loop through all of the current particle objects contained wihtin the vector attribute of the effect geenrator class 
		/*if (currentParticles[i].m_alpha <= 0.0f) {

			currentParticles.erase(currentParticles.begin() + i);
			currentParticles.resize(currentParticles.size() - 1);
		}*/
		currentParticles[i].move(dt); // for each particle update its postion and any toher attributes of the particle object such as an alpha value for its colour 
		
	}


}
