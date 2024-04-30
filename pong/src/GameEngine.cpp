#include "GameEngine.h"

GameEngine::GameEngine(sf::RenderWindow& window) 
	: m_window(window),
	m_paddle1(sf::Vector2f(20, window.getSize().y / 2.f), 10, 100, sf::Color::White),
	m_paddle2(sf::Vector2f(window.getSize().x - 20.f, window.getSize().y -100.f), 10, 100, sf::Color::White),
	m_ball(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f), 8, 400.f, sf::Color::White)
{   
	origin = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);
	m_p1Score = 0;
	m_p2Score = 0;
	m_gStates = GameStates::intro;
	m_font.loadFromFile(".\\assets\\fonts\\digital-7.ttf");
	m_hud.setFont(m_font);
	m_hud.setCharacterSize(50);
	m_hud.setFillColor(sf::Color::White);

	m_hud.setPosition((m_window.getSize().x / 2.f) - 45.f, 10);

	m_paddle1.setSpeed(1000.f);
	
}

void GameEngine::draw()
{
	m_window.clear();
	m_paddle1.draw(m_window);
	m_paddle2.draw(m_window);
	m_ball.draw(m_window);
	m_window.draw(m_hud);
	m_window.display();
}

void GameEngine::update()
{
	// update hud
	std::stringstream ss;
	switch (m_gStates)
	{
	case GameEngine::intro:
		ss << "Press the Space\nkey to start";
		break;
	case GameEngine::playing:
		ss << m_p1Score << " - " << m_p2Score;
		break;
	case GameEngine::gameOver:
		if (m_p1Score > m_p2Score) {
			ss << "Player 1 wins";
		}
		else {
			ss << "Player 2 wins";
		}
		break;
	default:
		break;
	}
		
	m_hud.setString(ss.str());
}



void GameEngine::run()
{
	float dt;

	while (m_window.isOpen())
	{
		dt = m_clock.restart().asSeconds();

		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) m_window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				m_window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				m_gStates = GameStates::playing;


			
		
		}



		if (m_gStates == 1 ) { // check if the game should be running using the m_gStates enum variable


			if ( sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){ // using the SFML keyboard check if 
				m_paddle1.move(-dt , m_window.getSize().y);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)||sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				m_paddle1.move(dt, m_window.getSize().y);
			}

			m_ball.move(dt, m_window);
			if (m_paddle1.getBounds().contains(m_ball.getPosition())) { // if the global bounds of the paddle  has the ball current coordinates in its current range then invert the velocity of the ball 
				m_ball.updateVelocity(1);
			}
			if (m_paddle2.getBounds().contains(m_ball.getPosition())) { // if the global bounds of the paddle has the current ball coordinates in its current range then invert the velocity of the ball 
				m_ball.updateVelocity(-1);
			}
			




			if (m_ball.getPosition().x > m_paddle2.getShape().getPosition().x) { // check if the ball has gone passed the paddles position 
				m_ball.setPosition(origin.x, origin.y); // set the postion of the ball to the centre of the screen once the ball has passed the paddle(half the screen width and half the screen height)
				m_ball.updateVelocity(-1); // make the ball go in the opposite direction to the side it just scored on 
				m_p1Score++; // upadte the score 
			}
			if (m_ball.getPosition().x < m_paddle1.getShape().getPosition().x) { // similar process as described above but for when the ball passes the left/player paddle 
				m_ball.setPosition(origin.x, origin.y);
				m_ball.updateVelocity(1);
				m_p2Score++;
				
			}


			
		}



		// ADD YOUR CODE HERE !!!
		
		
		// update hud
		update();
		// draw shapes to screen
		draw();
	}
}
