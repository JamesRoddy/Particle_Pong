#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include "Paddle.h"
#include "Ball.h"

class GameEngine
{
private:
	sf::RenderWindow& m_window;
	sf::Font m_font;
	sf::Text m_hud;
	sf::SoundBuffer m_ballBuffer;
	sf::Sound m_ballSound;
	sf::Clock m_clock;


	Paddle m_paddle1;
	Paddle m_paddle2;
	Ball m_ball;

	int m_p1Score;
	int m_p2Score;
	static const int m_maxScore = 3;
	enum GameStates {intro, playing, gameOver};
	enum KeyStates{ W = 22,UP = 73,S=23,DOWN=74 };
	GameStates m_gStates;
	KeyStates m_KeyStates;
public:
	GameEngine(sf::RenderWindow& window);
	void draw();
	void update();
	void run();
	void setKeyStates(sf::Event event,float dt);
	
};

