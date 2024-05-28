#pragma once
#include "particle.h"
#include <vector>

// the effectsGenerator class will be used to managed each particle and update/draw it wihtin the window 
class EffectGenerator {
public:

	EffectGenerator(unsigned int width, unsigned int height); // constructor for the effectsGenerator class that takes in the current width and height of the window 
	void update(float dt); // will be called to update all the particles currenlty in the 'currentParticles' vector attribute of this class 
	void drawShapes(sf::RenderWindow &window); // same as update but used to render each particle to the screen 
	void drawVerticies(sf::RenderWindow& window);

	void generateConstellation(sf::Vector2f position,sf::Vector2f ballVelocity);
	void manageEvents(sf::Vector2f ballPos);
	void generateParticles(int newCount, float radius, sf::Vector2f position, bool hasAlpha, int speedMin, int speedMax); // used to gennerate a number of particles, with a random starting postion and colour 
	void generateEvent();
	void generateCollsionParticles(sf::Vector2f collsionPoint, int direction); // used to generate particles when the ball collides with the paddles 
	void clearParticle();
	
private:
	std::vector<Particle> currentParticles; // store each particle that is currenlty being drawn to the window 
	
	std::vector< sf::Vertex> verticies;

	sf::Time currentEvenetTimer;
	sf::Time m_eventInitialiseTime  = sf::seconds(15);
	sf::Time m_eventEndTime = sf::seconds(30);
	enum m_event {END,CONSTELLATION};
	int eventStart;
	sf::Clock eventTimer;
	sf::Time m_constellationGenerationTime ;
	m_event m_newEvent;
	bool hasEvent = false;
	unsigned int m_windowWidth;// attributes used to keep track of windows width and height so particles arent drawn of screen 
	unsigned int m_windowHeight;





};