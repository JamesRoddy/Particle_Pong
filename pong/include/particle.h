#pragma once
#include <SFML/Graphics.hpp>


// classes that will be used to generate certain particle effects based on events such as the ball colliding with the paddle 
// in depth explanations for each of these classes and thier methods can be found in the particle.cpp file 
class Particle {
// public atttributes for the particle 
public:
	// defining constructor  for particle and ist arguments 
	Particle(float fX, float fY, sf::Color colour, float fRadius, bool bHasAlpha, int iSpeedMin , int iSpeedMax );
	void update(float fDt); /// used to update the particles postion in accordance with dt
	void draw(sf::RenderWindow& window); // draw method that will be used to render the shape attribute of each particle
	
	sf::FloatRect getBounds();
	sf::CircleShape getShape(); // get the circle shape object associated wiht the particle object allowing for manipluation outside of the particle class through built in sfml methods such as move and getPostion
	float getAlpha(); // used to access the private m_alpha attribute when needed 
	float getSpeed();
	bool hasCollided(sf::FloatRect fBounds);
	void setVelocity(sf::Vector2f fVelocityVec);

	int m_event;
	


private:
	float m_particleX;
	float m_particleY;
	float m_speed; // speed attribute used to control the rate at which the particle will move 
	bool m_hasAlpha = false; // definng if the alpha value of the colour should be manipluated 
	float m_alpha = 255.0f; // inital alpha 

	sf::Color m_colour; // colour attribute used to determine the appearance of the particle 
	sf::CircleShape m_particleShape; // defining the shape object that will be used to represent the particle(in this case the built in circleShape class is used to create the object) 
	sf::Vector2f m_velocity; // defining a sf::vector2f obejct that will be used to manipluate the postion of the sf::circleShape assigined to the m_particleShape attribute 
	



};

