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

	void generateConstellation();
	void generateParticles(int newCount, float radius, sf::Vector2f position); // used to gennerate a number of particles, with a random starting postion and colour 

	void generateCollsionParticles(sf::Vector2f collsionPoint, int direction); // used to generate particles when the ball collides with the paddles 
	void clearParticle();
	int debug = 0;
private:
	std::vector<Particle> currentParticles; // store each particle that is currenlty being drawn to the window 
	sf::VertexArray verticies;

	unsigned int m_windowWidth;// attributes used to keep track of windows width and height so particles arent drawn of screen 
	unsigned int m_windowHeight;





};