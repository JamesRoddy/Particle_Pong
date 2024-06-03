#include "GameEngine.h"

GameEngine::GameEngine(sf::RenderWindow& window)
// initilasing all of the attributes that are objects of other classes within the game engine.h file 
	: m_window(window),
	m_paddle1(sf::Vector2f(20, window.getSize().y / 2.f), 10, 100, sf::Color::White),  // initialsing the m_paddle1 class 
	m_paddle2(sf::Vector2f(window.getSize().x - 20.f, window.getSize().y - 100.f), 10, 100, sf::Color::White),
	m_ball(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f), 8, 400.0f, sf::Color::White),
	m_effects(window.getSize().x, window.getSize().y),
	m_powerUpsManager(window.getSize().x,window.getSize().y, ".\\assets\\fonts\\impact.ttf")

{   
	srand(time(0)); // set the seed for the sequnce of random numbers for the rand() function to generate(used to randomise things such as coodrinate postions)
	origin = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f); /// used for setting objects postion at the centre of the window
	m_p1Score = 0; // keep track of player scores
	m_p2Score = 0;
	m_gStates = GameStates::intro; // on intialisation set the game states enum vairbale to the value of 0
	m_font.loadFromFile(".\\assets\\fonts\\digital-7.ttf");  // used to load the specifc font file that will be used when text is rendered to the screen 
	m_hud.setFont(m_font); // setting the font of the m_hud attribute within the GameEngine class whihc is an object of the Text class 
	m_hud.setCharacterSize(50); 
	m_hud.setFillColor(sf::Color::White);
	m_ballBuffer.loadFromFile(".\\assets\\audio\\beep.flac");
	m_ballSound.setBuffer(m_ballBuffer);
	m_gameWinBuffer.loadFromFile(".\\assets\\audio\\winSound.wav");
	m_gameLooseBuffer.loadFromFile(".\\assets\\audio\\looseSound.wav");


	m_hud.setPosition((m_window.getSize().x / 2.f) - 45.f, 10);

	m_paddle1.setSpeed(1000.f);
	
}

void GameEngine::draw()
{
	// draw all shapes and text to the screen
	m_window.clear(); // refresh window for each call to draw ready for the next frame
	m_effects.drawShapes(m_window); // draw all current particle effects to the screen 
	m_effects.drawEventText(m_window);
	
	m_powerUpsManager.draw(m_window); // draw all power ups to the screen and their pop up text
	m_powerUpsManager.drawPowerUpText(m_window);
	// draw the hud ball and paddles
	m_paddle1.draw(m_window);
	m_paddle2.draw(m_window);
	m_ball.draw(m_window);
	m_window.draw(m_hud);
	m_window.display(); // display everything to the screen once it has been rendered 
}

void GameEngine::update()
{
	// update hud
	std::stringstream ss; // string stream that will hold all of the text currently being displayed on the screen 
	switch (m_gStates) // switch case used to determine how the hud should change based on the current value of the enum objet m_gamestates 
	{
	case GameEngine::intro: // if the value of game states has the same value as the enum constant 'intro'(0)
		ss << "Press the Space\nkey to start";
		m_effects.resetEventTimer(); // reset the event timers for particle effects
		m_powerUpsManager.resetTimers();// reset the  timers for power ups 
		break;
	case GameEngine::playing: 
		ss << m_p1Score << " - " << m_p2Score;
		break;

	case GameEngine::gameOver:
		if (m_p1Score > m_p2Score) {

			ss << "You win !\n";

		
		}
		else {
			ss << "Computer wins...\n";
			

		}
		
		// allow player to continue after winning or losing 
		
		ss << "would you like to\n continue(y/n)\n";  // added continue message during the game over screen
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {  // if the m_gameStates is currently set to the enum constant "gameOver" and the user presses the Y key
			m_gameEndSound.stop();// ensure the end game sound doesnt continue past the game over screen
			m_gStates = intro;  // we take them back to the intro state/screen allowing them to play again(rather than just closing the program

			// reset game objects to their original positions
			m_ball.resetPos( m_window.getSize().x / 2, m_window.getSize().y / 2); 
			m_paddle1.reset(sf::Vector2f(20, m_window.getSize().y / 2));
			m_paddle2.reset(sf::Vector2f(m_window.getSize().x - 20.f, m_window.getSize().y - 100.f));

			m_p1Score = 0;   // also make sure to reset the scores(so it doesnt keep asking them if they want to continue 
			m_p2Score = 0;  // otherwise the gameStates variable would keep getting set to the  gameOver constant 

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) { // if the user presses N during the game over screen

			m_window.close(); // we close the window 
		}
		

		break;

	default:
		break;
	}
		
	m_hud.setString(ss.str()); // assigining the string stream to the m_hud attribute that is an object of the built in SFML Text class that allows text to be drawn to the window
}


void GameEngine::run()
{
	float dt;
	
	while (m_window.isOpen())
	{
		dt = m_clock.restart().asSeconds(); // get the differnce between the last and current frame(delta time)  
		//alllowing better control of how the game updates interms of moving objects etc(ensuring that it is more consistent across different frame rates)

		sf::Event event;// creating an event object tied to the smfl library that allows handling of things such as key presses 
		while (m_window.pollEvent(event)) 
		{
			// using the window object's pollEvent method as the condition for the while loop continously 
			//checking for certain events such as keyboard input or the window closing  
			
			if (event.type == sf::Event::Closed) m_window.close(); 
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				m_window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && m_gStates!=gameOver)
				m_gStates = GameStates::playing;
		}
		


		// ADD YOUR CODE HERE !!!

		if (m_gStates == 1 ) { // check if the game should be running using the m_gStates enum variable
			
			// allow the user to move the left paddle with both arrow keys and W/S
			if ( sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){ // using the SFML isKeyPressed method from the keyBoard class to detect when a specifc key is pressed based on the enum for the key passed in
				m_paddle1.move(-dt , m_window.getSize().y); // if the condtion above is true move the player up
			
			
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				m_paddle1.move(dt, m_window.getSize().y);// if the condtion above is true move the player down
				
			}

			// get the ball to  move 
			m_ball.move(dt, m_window); 
			m_ball.increaseSpeed(dt);
			// below is a tracking method created for the paddle class which
			//allows the AI paddle to track the ball based on the vector between the ball and the paddle 
			/// get the second paddle to track the ball 
			m_paddle2.trackBall(m_ball.getPosition(), m_ball.getVelocity(), dt, m_window.getSize().y);
			
			
			//// collsion detection  for both paddles 
			if (m_ball.ballCollisionPushBack(m_paddle1.getShape(),dt)) { // if the global bounds of the paddle contain the balls position meaning that the two bounding rectangles would over lap
				m_ballSound.play();
				m_effects.generateCollsionParticles(m_ball.getPosition(), 1, m_paddle1.getShape().getFillColor()); // generate particles when the ball collides with the paddle defining the postion they start at and the direction of movement(negative or positive)
			   
			}
			if (m_ball.ballCollisionPushBack(m_paddle2.getShape(),dt)){ // if the global bounds of the paddle contain the balls position meaning that the two bounding rectangles would over lap 
				m_ballSound.play();
				m_effects.generateCollsionParticles(m_ball.getPosition(), -1, m_paddle2.getShape().getFillColor()); // generate particles when the ball collides with the paddle defining the postion they start at and the direction of movement(negative or positive)
			}
			
			// updating scores when the ball passes either paddle 

			if (m_ball.getPosition().x > m_window.getSize().x) { // check if the ball has gone passed the paddles position 
				int inewPosY = (rand() % (m_screenRandomBoundUpper - m_screenRandomBoundLower) + m_screenRandomBoundLower); // get a new random y postion for the ball to start at 
				m_ball.resetPos( origin.x, inewPosY); // reset balls postion and speed
				m_p1Score++;  // increment the score attribute of the m_paddle1 object(as it just scored)
				m_paddle2.aiValidateScore(m_p1Score, m_p2Score, m_maxScore);

			}
			if (m_ball.getPosition().x < 0) { // similar process as described above but for when the ball passes the left/player paddle 
				int inewPosY = (rand() % (m_screenRandomBoundUpper - m_screenRandomBoundLower) + m_screenRandomBoundLower);
				m_ball.resetPos( origin.x, inewPosY); // reset balls postion
				m_p2Score++;
				m_paddle2.aiValidateScore(m_p1Score, m_p2Score, m_maxScore);
			}

			

			m_powerUpsManager.generatePowerUp(); // control when power ups spawn
			m_powerUpsManager.update(dt); // update power up positions 
			m_powerUpsManager.handleCollision(&m_ball,&m_paddle1,&m_paddle2); // handle collisions and effect application 
			m_powerUpsManager.manageDurationEffects(&m_ball,dt); // handle duration effects
			m_powerUpsManager.updatePopUpText(dt);


			m_effects.generateEvent(); // when particle effect event should trigger
			m_effects.manageEvents();// manage when particle event triggers
			m_effects.update(dt); // update all particles currently on screen 
			m_effects.handleParticleCollisions(&m_ball);
			
			
			// checking for if either paddle has a score equal to the m_maxScore variable
			if (m_p1Score == m_maxScore || m_p2Score == m_maxScore) { // check if either score attribute attached to the paddle 1 and paddle 2 objects has reached the max score count 

				m_gStates = gameOver; // if so set the current value of m_gamestates to the constant "gameOver" defined in the enum type gameStates(in the GameEngine header file)
				m_effects.clearParticle(); // clear all particles if there are any remaining on screen 

				m_effects.resetEventTimer();
				// reset powerups
				m_powerUpsManager.clearPowerUps(&m_ball);
				m_powerUpsManager.resetTimers();
				// detminign which sound buffer should be allocated to the m_gameEndSound attribute 
				m_p1Score > m_p2Score ? m_gameEndSound.setBuffer(m_gameWinBuffer) : m_gameEndSound.setBuffer(m_gameLooseBuffer);
				m_gameEndSound.play();
			}

			
		}
		
		// update hud
		update();
		// draw shapes to screen
		draw();
	}
}
