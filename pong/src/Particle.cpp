#pragma once
#include "particle.h"
#include<iostream>
#include<Ball.h>

// a particle class that defines a circular particle with a speed range, alpha value, radius, position, and color,
Particle::Particle(float startX, float startY, sf::Color colour, float radius, bool hasAlpha, int speedMin , int speedMax,bool bHasCollsion ) {

	
	m_particleX = startX; // set the m_particle X and y attributes of the particle class the the startX and startY psotions passed into the constructor 
	m_particleY = startY;
	m_hasCollsion = bHasCollsion; // determine if the particle is able to collide with other objects 

	m_speed = float((rand() % (speedMax - speedMin) + speedMin)); // define the speed of the particle by geernating a random value bewteen the speed range min and max(including those numbers)
	m_velocity.x = float((rand() % (speedMax - speedMin) + speedMin)); /// set the velcoity vector's x and y to the speed generated 
	m_velocity.y = float((rand() % (speedMax - speedMin) + speedMin)); // this vector is what will be used to move each particle on the screen 
	m_hasAlpha = hasAlpha; // some particles may have their alpha values manipulated therefore this bool attribute 'm_hasAlpha' will be used to determine if the particles alpha should be changed 
	m_colour = colour;  // set the colour attribute of the particle 
	m_event = 0; // used to control what effects are applied to the particle

	m_fadeMultiplier = 120.0f; // used to apply fading to particle at a particualr rate
	m_alpha = 255.0f; // intial alpha
	m_alphaCollsionVal = 85.0f;// minumum alpha required for collision
	m_collionSpeedDecrease = 0.95f; // used to decrease on objects speed when it collides with a particle 
	// setting the properties of the particle's shape object(in this case the in built sf::CircleShape object)
	m_particleShape.setRadius(radius); // set the radius of the particle using the radius argument passed into the constructor  
	m_particleShape.setPosition(m_particleX, m_particleY); //set pos
	m_particleShape.setFillColor(colour);// set colour 
	m_particleShape.setOrigin(m_particleShape.getRadius() / 2, m_particleShape.getRadius() / 2); // set origin based on radius 
	

}

bool Particle::hasCollided(Ball*ball) {
	// if the alpha value is greater than the minumu alpha required for collsion and we have an intersection between bounds
	if (m_alpha >= m_alphaCollsionVal && m_particleShape.getGlobalBounds().intersects(ball->getShapeReference()->getGlobalBounds())) { 
		return true; // return true
	}
	
	return false; // otherwise return false as there was no collsion or the particles alpha wasnt high enough for one ot take place 
}

bool Particle::getCollision() const {
	return m_hasCollsion;
}
float Particle::getSpeedDecrease() const {
	return m_collionSpeedDecrease;
}
void Particle::update(float dt) { // method of the particle class used to update the pos of the particles shape and any other arttibutes that need to be updated i.e alpha values

	
	if (m_hasAlpha) { // if the particles alpha value should be manipulated 
		m_colour.a = m_alpha; // set the particles color attribute to the current value of its alpha attriubte m_alpha(has a default value of 255 for each particle)
		m_particleShape.setFillColor(m_colour); // set the fill colour of the particles shape to take into account the new alpha value 

		if (!(m_alpha <= 0.0f)) {  // if the alpha values is not smaller than or equal to 0(ensuring the particles dont reappear after fading)
			
			m_alpha -= dt*m_fadeMultiplier; // tick down the alpha value of the particle by a small amount, resulting a fading effect over time 
			
		}
	}
	m_particleShape.move(m_velocity * dt); // get the particles shape attribute  and call the predefined move method 
	// moving the particle's shape  at on offset of its velocity vector attribute multiplied by dt to move it at a consistent rate 
	

}
void Particle::draw(sf::RenderWindow& window) const { // the draw method for the particle that will be called in a foreach loop to draw each particle to the screen(this loop can be located in the effectGenerator class)

	window.draw(m_particleShape); // call the draw method through the refernce argument to the window to draw the particles shape(an attribute of the particle class)
	

}

// getters and setters that allow an access point to the properties of each particle object created
// these will be used by the effect Generator class to get various attributes related to each partilce it generates
// along with setting attributes such as velocity of each partilce on generation
sf::CircleShape Particle::getShape() {

	return m_particleShape;
}
sf::FloatRect Particle::getBounds() {
	return m_particleShape.getGlobalBounds();
}
float Particle::getSpeed() const {

	return m_speed;
}


float Particle::getAlpha() const { 

	return m_alpha;
}



void Particle::setVelocity(sf::Vector2f fVelocityVec) {
	m_velocity = fVelocityVec;
}






