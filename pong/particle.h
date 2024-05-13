#include <SFML/Graphics.hpp>
#include<vector>

// classes that will be used to geenrate certain particle effects based on events such as the ball colliding with the paddle 
// in depth explanations for each of these classes and thier methods can be found in the particle.cpp file 
class particle {
public:
	// definig constrcuor  for particle and ist arguments 
	particle(float x, float y, sf::Color colour, float radius , bool hasAlpha,int speedMin = 100 ,int speedMax = 200);
	void update(float dt); /// used to update the particles postion in accordance wiht dt
	void draw(sf::RenderWindow& window); // draw method that will be used to render the shape attribute of each particle
	// public atttributes for the particle 

	sf::CircleShape m_particleShape; // defining the shape object that will be used to represent the particle(in this case the built in circleShape class is used to create the object) 
	sf::Vector2f m_velocity; // defining a sf::vector2f obejct that will be used to manipluate the postion of the sf::circleShape assigined to the m_particleShape attribute 
	sf::Color m_colour; // colour attribute used to determine the appearance of the particle 

	float m_speed; // speed attribute used to control the rate at which the particle will move 
	bool m_hasAlpha = false; // definng if the alpha value of the colour should be manipluated 
	float m_alpha = 255.0f; // inital alpha 

private:
	float m_particleX;
	float m_particleY;





};


// the effectsGenerator class will be used to managed each particle and update/draw it wihtin the window 
class effectGenerator {
public:
	effectGenerator(unsigned int width, unsigned int height); // constrcor for the effectsGenerator class that takes in the current width and height of the window 

	void update(float dt); // will be called to update all the particles currenlty in the 'currentParticles' vector attribute of this class 
	void draw(sf::RenderWindow& window); // same as update but used to render each particle to the screen 

	void generateParticles(int newCount, float radius,sf::Vector2f position); // used to gennerate a number of particles , with a random starting postion and colour 
	void generateCollsionParticles(sf::Vector2f collsionPoint, int direction); // used to generate particles when the ball collides with the paddles 

private:
	std::vector<particle> currentParticles; // store each particle that is currenlty being drawn to the window 
	unsigned int windowWidth ; // attributes used to keep track of windows width and height so particles arent drawn of screen 
	unsigned int windowHeight ;
	




};