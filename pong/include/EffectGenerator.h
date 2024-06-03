#pragma once
#include "particle.h"

#include <vector>
#include<sstream>
// the effectsGenerator class will be used to managed each particle and update/draw it wihtin the window 
class EffectGenerator {
public:

	EffectGenerator(float fWindowWidth,float  fWindowHeight); // constructor for the effectsGenerator class that takes in the current width and height of the window 
	void update(float fDt); // will be called to update all the particles currenlty in the 'currentParticles' vector attribute of this class 
	
	
	void drawShapes(sf::RenderWindow &window); // same as update but used to render each particle to the screen 

	bool handleParticleCollisions(sf::FloatRect fBounds); // handle collsions between ball and particle 
	void generateExplosion(); // generate particle explosions on the screen
	
	void generateEvent();// manage when events should trigger 
	void manageEvents(); // spawn events
	void setEventDisplayText();

	void generateParticles(const int iNewCount, float frRadius, bool bHasAlpha, int iSpeedMin, int iSpeedMax,float fX,float fY); // used to gennerate a number of particles with random properties 
	void drawEventText(sf::RenderWindow &window);
	void generateCollsionParticles(sf::Vector2f fCollsionPoint, int fDirection); // used to generate particles when the ball collides with the paddles 
	
	int generateRandomValue(int iUpperBound, int iLowerBound); // used to geenrate various random properties such as explosion amounts and particle sizes
	float generateRandomValue(float iUpperBound, float iLowerBound);
	
	void clearParticle(); // clear all particles from the screen
	void resetEventTimer();

private:
	std::vector<Particle> m_currentParticles; // store each particle that is currenlty being drawn to the window 

	sf::Clock m_eventTimer; // used to get the elapsed time since the game started
	sf::Time m_explosionGenerationTime; // used to control when explosions are triggered
	sf::Time m_currentEvenetTimer; // keep track of current elpased time
	sf::Time m_eventInitialiseTime; // used to control at what point events are initialised
	sf::Time m_displayTime; // used to dispaly the time until/duration of the event
	sf::Time m_eventEndTime ;// used to control when events end
	// displaying the time until/duration of events
	sf::Font m_eventTextFont;
	sf::Text m_eventText;
	std::stringstream m_eventDisplayText ;
	unsigned int m_eventTextSize;
	float m_eventTextOffset;
	int m_explosionAmount; 
	enum events {END,PARTICLESTORM}; // used to control what event should fire 
	events m_newEvent;
	bool m_hasStartIndex;
	int m_eventStartIndex; // get starting index of the event i.e the first particle that was generated during the event
	bool m_hasEvent; // boolean flag for events generating 

	float m_windowWidth;// attributes used to keep track of windows width and height so particles arent drawn of screen 
	float m_windowHeight;





};