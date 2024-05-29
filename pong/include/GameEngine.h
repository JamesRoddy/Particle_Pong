#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include "Paddle.h"
#include "Ball.h"
#include "EffectGenerator.h"

class GameEngine
{
private:
	
	// creating all of the attributes that are objects of built in SFML classes 
	sf::RenderWindow& m_window;  // refernce attribute to the window object that will be passed into the GameEngine constructor(by reference)
	sf::Font m_font; // creating an attribute that is an object of the font class allowing for loading of font files
	sf::Text m_hud;  // creating an attribute that is of the Text class that can have a font assigned to it and this object can then be draw to a window my setting its text content using the setString method 
	sf::SoundBuffer m_ballBuffer;
	sf::Sound m_ballSound;
	sf::Clock m_clock; // creating an object of the SFML Clock class that will be used to generate values such as delta time within the main game loop 

	// all attribuets that are objects of another class that will also need to be instantiated when an object of the GameEngine class is created  
	EffectGenerator m_effects;  
	Paddle m_paddle1;
	Paddle m_paddle2;
	Ball m_ball;
	std::vector<Particle> particlesWithCollision;
	
	
	sf::Vector2f origin; /// attribute to store the centre of the window when the GameEngine class is initialised used to reset the postion of objects such as the ball when it goes off screen
	int m_p1Score; // used to keep track of the score for each paddle 
	int m_p2Score;
	int m_screenRandomBoundLower = m_window.getSize().y/2+50; // random range for spawn point of the ball each time the player or AI scores 
	int m_screenRandomBoundUpper = m_window.getSize().y/2 - 50;
	static const int m_maxScore = 5; // a constant vairiable that defines the maximum score(the value of this variable cannot be edited/changed aside from where it is defined)
	enum GameStates {intro, playing, gameOver};// an enum type is defined that is used to control what should be dislayed to the player and the overall state of the game 
    // enums are a special set of defined constants(integer values only) in this case it provides a group of constants to represent game states 
	
	GameStates m_gStates; /// creating an attribute of the GameEngine class that has the enum type GameStates giving thi spsecifc attribute access to the set of constants defined by that enum type 
	
public:
	GameEngine(sf::RenderWindow& window);
	void draw();
	void update();
	void run();
	
	
};

