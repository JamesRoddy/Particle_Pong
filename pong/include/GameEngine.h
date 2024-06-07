#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include "Paddle.h"
#include "Ball.h"
#include "EffectGenerator.h"
#include "PowerUpsManager.h"
#include "Menu.h"

class GameEngine
{
private:
	
	// creating all of the attributes that are objects of built in SFML classes 
	sf::RenderWindow& m_window;  // refernce attribute to the window object that will be passed into the GameEngine constructor(by reference)
	sf::Font m_font; // creating an attribute that is an object of the font class allowing for loading of font files
	sf::Text m_hud;  // creating an attribute that is of the Text class that can have a font assigned to it and this object can then be draw to a window my setting its text content using the setString method 
	// sound buffers and sound effects 
	sf::SoundBuffer m_gameLooseBuffer;
	sf::SoundBuffer m_gameWinBuffer;
	sf::Sound m_gameEndSound; // used to switch between win and loose music depending on the player score at the end of the game
	sf::Music m_gameLoopMusic;
	sf::Music m_introMusic;
	sf::SoundBuffer m_scoreSoundBuffer;
	sf::Sound m_scoreSound;

	sf::SoundBuffer m_ballBuffer; // used for loading the ball collision sound effect
	sf::Sound m_ballSound;
	sf::Clock m_clock; // creating an object of the SFML Clock class that will be used to generate values such as delta time within the main game loop 

	// all   attributes that are objects of another class that will  need to be instantiated when an object of the GameEngine class is created  
	EffectGenerator m_effects;  
	PowerUpsManager m_powerUpsManager;
	Menu m_menu;
	Paddle m_paddle1;
	Paddle m_paddle2;
	Ball m_ball;
	
	sf::Vector2f origin; /// attribute to store the centre of the window when the GameEngine class is initialised used to reset the postion of objects such as the ball when it goes off screen
	int m_p1Score; // used to keep track of the score for each paddle 
	int m_p2Score;
	float m_screenRandomBoundLower = m_window.getSize().y/2.0f + 50.0f; // random range for spawn point of the ball each time the player or AI scores 
	float m_screenRandomBoundUpper = m_window.getSize().y/2.0f - 50.0f;
	bool m_shouldDrawMenu;
	static const int m_maxScore = 7; // a constant vairiable that defines the maximum score(the value of this variable cannot be edited/changed aside from where it is defined)
	enum GameStates {intro, playing, gameOver,menu};// an enum type is defined that is used to control what should be dislayed to the player and the overall state of the game 
    // enums are a special set of defined constants(integer values only) in this case it provides a group of constants to represent game states 
	
	GameStates m_gStates; /// creating an attribute of the GameEngine class that has the enum type GameStates giving thi spsecifc attribute access to the set of constants defined by that enum type 
	
public:
	GameEngine(sf::RenderWindow& window);
	void resetGame();
	void draw();
	void update();
	void run();
	
	
};

