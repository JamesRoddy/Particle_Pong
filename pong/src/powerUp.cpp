
#include "powerUp.h"
#include<iostream>


powerUp::powerUp(float windowWidth,float windowHeight){


	
	m_id = rand() % (INCREASEPADDLESIZE + 1); // generate random effect id using the enum "m_effects" declared in the header file for the power up class(adding 1 because the upper bound for rand is exclusive)
	m_speed = 300.0f; // set speed for powerups allowing them to move around on screen
	m_windowHeight = windowHeight;// window width and height 
	m_windowWidth = windowWidth;
	m_maxTargetHitCount = 6; // total amount of times the power up can hit its movement target before disappearing
	m_targetHitCount = 0; // track each time the power up successfully moves to its targe
	m_powerUpScaleFactor = 1.5f;
	m_ballSpeedMultiplier = 80.0f;
	m_tickUpTime = sf::seconds(0.001f); // time that will be applied to the power up current duration each frame controllling how fast the duration effect of the power up will fade
	m_shape.setSize(sf::Vector2f(20.0f, 20.0f));// set the size of the powerup
	m_shape.setOrigin(m_shape.getSize().x / 2, m_shape.getSize().y / 2); 
	

}


void powerUp::setSpawn() {
	float boundUpperX = m_windowWidth - m_windowHeight / 4; // bounds for spawning and moving power ups in x
	float boundLowerX = m_windowWidth / 4;

	// generatre initial spawn in the x axis 
	float spawnX = boundLowerX + static_cast<float>(rand()) / static_cast<float>((RAND_MAX / (boundUpperX - boundLowerX))); // generate random initial postion for the explosions withn the bounds of the screen
	// generate initial target y for the power up to move to
	float targetY = m_shape.getSize().y * 2 + static_cast<float>(rand()) / static_cast<float>((RAND_MAX / ((m_windowHeight - m_shape.getSize().y * 2) - m_shape.getSize().y * 2)));
	m_shape.setPosition(spawnX, 0 - m_shape.getSize().y);// spawn location for power ups(always above the screen)

	m_target = sf::Vector2f(spawnX, targetY); //initial target for movement

}

void powerUp::applyEffect(Ball* ball,float dt) {

	switch (m_id) // control what effect the power up applies via its id that is assigned randomly to one of the enum  constants in the constructor
	{
	  case INCREASEPADDLESIZE: 
		  paddleRef->getShapeReference()->setScale(m_powerUpScaleFactor, m_powerUpScaleFactor);
		  paddleRef->getShapeReference()->setFillColor(m_colour); // set the colour to the colour of the power up
		
		//std::cout << paddleRef->getShapeReference()->getSize().y << std::endl;
		break;

	  case INVERTVELOCITY:
		ball->setVelocity(-ball->getVelocity()); // invert the velocity of the ball 

		break;
	  case INCREASEBALLSPEED:
		ball->setSpeed(ball->getSpeed() + dt * m_ballSpeedMultiplier); // increase speed of the ball be dt * m_ballSpeedMultipler
		ball->getShapeReference()->setFillColor(m_colour);
		break;

	}
	m_currentEffectTime += m_tickUpTime; // increase the current effect time of the powerup(until it reaches the total effect time attribute
	
	
}



bool powerUp::negateEffect(Ball* ball) {

	// used to negate any duration effects after they have finished 
	if (m_currentEffectTime.asSeconds()  >= m_totalEffectTime.asSeconds()) { // if the power up duration is greater than its total
		switch (m_id) { // control what effect needs to be negated via the id of the power up
		case INCREASEPADDLESIZE: 
			
			paddleRef->getShapeReference()->setScale(1.0f,1.0f); // decrease scale of paddle assigned to power up if it has the increase size effect
			paddleRef->getShapeReference()->setFillColor(sf::Color::White);//set the colour of the paddle assigned to the power up back to white 

			break;
		
		case INCREASEBALLSPEED:
			ball->getShapeReference()->setFillColor(sf::Color::White);
			break;
		}

		return true; // return true as the effect has been negated and we have hit our total effect time
	}
	return false; // otherwise retunr false as we havent hit our total effect time
}


void powerUp::setPaddle(Paddle* paddleReference) { // used to get a refernce to the paddle the effect needs to be applied to 
	paddleRef = paddleReference;
}


void powerUp::setTotalEffectTime(sf::Time newEffectTime) {
	m_totalEffectTime = newEffectTime;
}

sf::Vector2f  powerUp:: randomTarget() { // will assign a random target to the power up for it to move to each time it hits its current target
 
 // the following code will generate a random postion for the power up  to move to ensuring that it remains 
 // inside of the screen while doing so
  float boundLowerY = m_shape.getSize().y * 2; 
  float boundLowerX = m_shape.getSize().x * 2;

  float targetY = boundLowerY * 2 + static_cast<float>(rand()) / static_cast<float>((RAND_MAX / ((m_windowHeight - boundLowerY) - boundLowerY)));
  float targetX = boundLowerX * 2 + static_cast<float>(rand()) / static_cast<float>((RAND_MAX / ((m_windowWidth - boundLowerX) - boundLowerX)));

	return sf::Vector2f(targetX, targetY); // return the new target
}

void powerUp::drawPowerUp(sf::RenderWindow& window) { 

	window.draw(m_shape); // draw the power ups to the screen
}

   
bool powerUp::collision(sf::FloatRect bounds) {
	// used to check collsion between power ups and the ball
	if (m_shape.getGlobalBounds().intersects(bounds)) { 
		return true;
		m_hasCollided = true;
	}
	return false;

}


// getting max maz target hit count and current movement target hit count for the power ups 
int  powerUp::getTargetHitCount() { 
	return m_targetHitCount;
}
int powerUp::getMaxTargetHitCount() {
	return m_maxTargetHitCount;
}
// used to modify the duration of the power up effect if a power up with the same effect is currently active
void powerUp::incrementDuration() { 

	m_totalEffectTime+=m_totalEffectTime*0.25f;  /// increase over all effect duration by 25%

}
// get the time of collsion between the power up and ball
void powerUp::setTimeOfCollision(sf::Time newTimeOfCollision) { // set initial total effect time
	if (m_totalEffectTime.asSeconds() != 0.0f) {
		m_totalEffectTime = m_totalEffectTime + newTimeOfCollision; // setting  the total effect time to the time of collsion plus the set effect time
		std::cout << newTimeOfCollision.asSeconds() << std::endl;
		std::cout << m_currentEffectTime.asSeconds() << std::endl;
	}
	
}
sf::Time powerUp::getDuration() { // retunr how long the power up willl last for in total
	return m_totalEffectTime;
}


void powerUp::setType(sf::Color colour,sf::Time effectTime) { // set the type of the power up i.e its effect time and colour
	m_shape.setFillColor(colour);
	
	m_totalEffectTime = effectTime;
	m_colour = colour;

}


int powerUp::getId() { // get id of the power up
	return m_id;
}
void powerUp::updatePowerUpPos(float dt) { // used to update the power up and move it around the screen

	sf::Vector2f distanceToTarget = m_target - m_shape.getPosition();// get distance between power up and current target
	float length = sqrt(distanceToTarget.x * distanceToTarget.x + distanceToTarget.y * distanceToTarget.y);// get lenght of distance
	distanceToTarget.x /= length; // normalise the distance vector so we have adirection
	distanceToTarget.y /= length;
	if (!m_shape.getGlobalBounds().contains(m_target)) { // if we have not  reached our target
		m_shape.move(distanceToTarget*(m_speed*dt));// move along the normalised direction vector

	}
	else { // otheriwse we have hit our target
		m_targetHitCount++; // increment our current target hit count used to manage how long the power up stays on screen
		m_target = randomTarget(); // assigned a new random target within a specifc bound range 
	}
	


}


sf::RectangleShape powerUp::getShape() {
	return m_shape;
}