#pragma once
#include "particle.h"
#include <SFML/Audio.hpp>
#include <vector>
#include<sstream>
// the effectsGenerator class will be used to managed each particle and update/draw it wihtin the window 
class EffectGenerator {
public:

	EffectGenerator(float fWindowWidth,float  fWindowHeight); // constructor for the effectsGenerator class that takes in the current width and height of the window 
	void update(float fDt); // will be called to update all the particles currenlty in the 'currentParticles' vector attribute of this class 
	
	
	void drawShapes(sf::RenderWindow &window); // same as update but used to render each particle to the screen 

	void handleParticleCollisions(Ball*ball); // handle collsions between ball and particle 
	void generateExplosion(); // generate particle explosions on the screen
	
	void generateEvent(float fDt);// manage when events should trigger 
	void manageEvents(); // spawn events
	void setEventDisplay(float fDt);

	void generateParticles(const int iNewCount, float frRadius, bool bHasAlpha, int iSpeedMin, int iSpeedMax,float fX,float fY,bool BhasCollsion); // used to gennerate a number of particles with random properties 
	void drawEventDisplay(sf::RenderWindow &window) const;
	void generateCollsionParticles(sf::Vector2f fCollsionPoint, int fDirection,sf::Color objectColour,int iAmount); // used to generate particles when the ball collides with the paddles 
	
	void clearParticle(); // clear all particles from the screen
	void resetEventTimer();

private:
	std::vector<Particle> m_currentParticles; // store each particle that is currenlty being drawn to the window 

	sf::Clock m_eventTimer; // used to get the elapsed time since the game started
	sf::Time m_explosionGenerationTime; // used to control when explosions are triggered
	sf::Time m_currentEvenetTimer; // keep track of current elpased time
	
	sf::Time m_eventInitialiseTime; // used to control at what point events are initialised
	sf::Time m_displayTime; // used to display the time until an event
	sf::Time m_eventEndTime ;// used to control when events will disipate
	sf::SoundBuffer m_warningSoundBuffer; 
	sf::Sound m_warningSound;
	sf::SoundBuffer m_particleExplosionBuffer;
	sf::Sound m_particleExplosionSound;

	sf::Font m_eventTextFont; // font for the event text display
	sf::Text m_eventText; // the text object that willl be assigned the font and event text
	

	std::stringstream m_eventDisplayText ; // string stream that will be set to the text object
	unsigned int m_eventTextSize; // text size for the event tetx display
	float m_eventTextOffset; // any offset in x or y that needs to be applied to the text
	
	sf::RectangleShape m_eventWarningSign;
	sf::Texture m_warningSignTexture;
	int m_explosionAmount; 
	
	enum events {END,PARTICLESTORM}; // used to control what event should fire 
	events m_newEvent;
	
	bool m_hasStartIndex;
	int m_eventStartIndex; // get starting index of the event i.e the first particle that was generated during the event
	float m_warningTextScalar;
	bool m_hasEvent; // boolean flag for events generating 
	bool m_warningShouldScale;
	float m_windowWidth;// attributes used to keep track of windows width and height so particles arent drawn of screen 
	float m_windowHeight;
    

	int generateRandomValue(int iUpperBound, int iLowerBound); // used to generate various random properties such as explosion amounts and particle sizes
	float generateRandomValue(float iUpperBound, float iLowerBound);
	void updateEventWarnings(float fDt); // used to display a warning sign to when an event gets close


};