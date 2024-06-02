#pragma once
#include "particle.h"

#include <vector>
#include<sstream>
// the effectsGenerator class will be used to managed each particle and update/draw it wihtin the window 
class EffectGenerator {
public:

	EffectGenerator(unsigned int width, unsigned int height); // constructor for the effectsGenerator class that takes in the current width and height of the window 
	void update(float dt); // will be called to update all the particles currenlty in the 'currentParticles' vector attribute of this class 
	
	
	void drawShapes(sf::RenderWindow &window); // same as update but used to render each particle to the screen 

	bool handleParticleCollisions(sf::FloatRect bounds); // handle collsions between ball and particle 
	void generateConstellationExplosion(); // generate particle explosions on the screen
	
	void generateEvent();// manage when events should trigger 
	void manageEvents(sf::Vector2f ballPosition, sf::Vector2f ballVelocity); // spawn events
	
	void generateParticles(const int newCount, float radius, bool hasAlpha, int speedMin, int speedMax,float x,float y); // used to gennerate a number of particles, with a random starting postion and colour 
	
	void generateCollsionParticles(sf::Vector2f collsionPoint, int direction); // used to generate particles when the ball collides with the paddles 
	
	void clearParticle(); // clear all particles from the screen
	void resetEventTimer();

private:
	std::vector<Particle> m_currentParticles; // store each particle that is currenlty being drawn to the window 

	sf::Clock m_eventTimer; // used to get the elapsed time since the game started
	sf::Time m_constellationGenerationTime; // used to control when constellations are triggered
	sf::Time m_currentEvenetTimer; // used to control when events are triggered
	sf::Time m_eventInitialiseTime; 
	sf::Time m_displayTime;
	sf::Time m_eventEndTime ;
	sf::Font m_eventTextFont;
	sf::Text m_eventText;
	std::stringstream eventDisplay ;
	sf::CircleShape m_debug;
	enum events {END,PARTICLESTORM}; // used to control what event should fire 
	events m_newEvent;
	int m_eventStartIndex; // get starting index of the event i.e the first particle that was generated during the event
	bool m_hasEvent; // boolean flag for events generating 

	unsigned int m_windowWidth;// attributes used to keep track of windows width and height so particles arent drawn of screen 
	unsigned int m_windowHeight;





};