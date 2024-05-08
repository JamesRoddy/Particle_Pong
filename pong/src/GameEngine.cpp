#include "GameEngine.h"

GameEngine::GameEngine(sf::RenderWindow& window) 
	: m_window(window),
	m_paddle1(sf::Vector2f(20, window.getSize().y / 2.f), 10, 100, sf::Color::White),
	m_paddle2(sf::Vector2f(window.getSize().x - 20.f, window.getSize().y -100.f), 10, 100, sf::Color::White),
	m_ball(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f), 8, 400.f, sf::Color::White)
{
	srand(time(0));
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
			ss << "Player 1 wins\n";
		
		}
		else {
			ss << "Player 2 wins\n";
			

		}
		// allow player to continue after winning or losing 
		
		ss << "would you like to\n continue(y/n)\n";  // added continue message during the game over screen
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {  // if the m_gameStates is currently set to the constant "gameOver" and the user presses the Y key
			
			m_gStates = intro;  // we take them back to the intro state/screen allowing them to play again(rather than just closing the program
			
			m_p1Score = 0;   // also make sure to reset the scores(so it doesnt keep asking them if they want to continue 
			m_p2Score = 0;  //i.e otherwise the gameStates vairbale would keep getting set to the  gameOver constant 

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) { // if the user presses N during the game over screen

			m_window.close(); // we close the window 
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
		dt = m_clock.restart().asSeconds(); // get the differnce between the last and current frame(delta time)  
		//alllowing better control of how the game updates interms of moving objects etc(ensuring that it is more consistent across different frame rates)

		sf::Event event;// creating an event object tied to the smfl libaray that allows handling of things such as key presses 
		while (m_window.pollEvent(event)) // using the window object's pollEvent method as the condition for the while loop continously checking for certain events such as keyboard input or the window closing  
		{

			if (event.type == sf::Event::Closed) m_window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				m_window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				m_gStates = GameStates::playing;


			
		
		}
		


		// ADD YOUR CODE HERE !!!

		if (m_gStates == 1 ) { // check if the game should be running using the m_gStates enum variable

			// allow the user to move the left paddle with both arrow keys and W/S

			if ( sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){ // using the SFML isKeyPressed method from the keyBoard class to detect when a specifc key is pressed based on the enum for the key passed in
				std::cout << "move" << std::endl;
				m_paddle1.move(-dt , m_window.getSize().y); // if the condtion above is true move the player up
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				m_paddle1.move(dt, m_window.getSize().y);// if the condtion above is true move the player down
			}


			// get the ball to  move 
			m_ball.move(dt, m_window); 
			m_paddle2.trackBall(m_ball.getPosition(), dt, m_window.getSize().y);// tracking method created for the paddle class 
			//allowing the AI paddle to track the ball based on the vector between the ball and the paddle 


			// collsion detection  for both paddles 

			if (m_paddle1.getBounds().contains(m_ball.getPosition())) { // if the global bounds of the paddle  has the ball current coordinates in its current range then invert the velocity of the ball 
				m_ball.updateVelocity(1);// we reverse the velocity of the ball to travel towards the right
			}
			if (m_paddle2.getBounds().contains(m_ball.getPosition())) { // if the global bounds of the paddle has the current ball coordinates in its current range then invert the velocity of the ball 
				
				m_ball.updateVelocity(-1); // we reverse the velcoity of the ball to travel towards the left
			}
			




			// updating scores when the ball passes either paddle 

			if (m_ball.getPosition().x > m_paddle2.getShape().getPosition().x) { // check if the ball has gone passed the paddles position 
				
				int newPosY = (rand() % (randomBoundUpper - randomBoundLower) + randomBoundLower);
				m_ball.resetPos(-1, origin.x, newPosY); // reset balls postion and speed
				m_p1Score++;  // increment the score attribute of the m_paddle1 object(as it just scored)
			}
			if (m_ball.getPosition().x < m_paddle1.getShape().getPosition().x) { // similar process as described above but for when the ball passes the left/player paddle 
				int newPosY = (rand() % (randomBoundUpper - randomBoundLower) + randomBoundLower);
				m_ball.resetPos(1, origin.x, newPosY); 
				
				m_p2Score++;
				
			}

			// checking for if either paddle has a score equal to the m_maxScore variable

			if (m_p1Score == m_maxScore || m_p2Score == m_maxScore) { // check if either score attribute attached to the paddle 1 and paddle 2 objects has reached the max score count 
				m_gStates = gameOver; // if so set the current value of m_gamestates to the constant "gameOver" defined in the enum type gameStates(in the GameEngine header file)
			   
			
			}

			
				

			
		}



		
		
		
		// update hud
		update();
		// draw shapes to screen
		draw();
	}
}
