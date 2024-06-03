
#include "powerUp.h"
#include<iostream>


powerUp::powerUp(float fWindowWidth,float fWindowHeight){


	m_id = rand() % (INCREASEPADDLESIZE + 1); // generate random effect id using the enum "m_effects" declared in the header file for the power up class(adding 1 because the upper bound for rand is exclusive)
	m_speed = 300.0f; // set speed for powerups allowing them to move around on screen
	m_windowHeight = fWindowHeight;// window width and height 
	m_windowWidth = fWindowWidth;
	m_direction = 0;
	m_maxTargetHitCount = 8; // total amount of times the power up can hit its movement target before disappearing
	m_targetHitCount = 0; // track each time the power up successfully moves to its targe
	m_durationScalar = 0.15f; // used to increase the duration of the power up allowing effects to stack
	// used to modify various objects the power up effects are applied to 
	m_powerUpScaleFactor = 1.5f;
	m_ballSpeedMultiplier = 65.0f;
	// determine if the power has a paddle ffect or ball effect 
	m_hasBallEffect = false;
	m_hasPaddleEffect = false;

	m_tickUpTime = sf::seconds(0.001f); // time that will be applied to the power up current duration each frame controllling how fast the duration effect of the power up will fade
	m_shape.setSize(sf::Vector2f(15.0f, 15.0f));// set the size of the powerup
	m_shape.setOrigin(m_shape.getSize().x / 2, m_shape.getSize().y / 2); 
	setEffectTarget();

}


void powerUp::setSpawn() {
	float fBoundUpperX = m_windowWidth - m_windowHeight / 4; // bounds for spawning and moving power ups in x
	float fBoundLowerX = m_windowWidth / 4;

	// generatre initial spawn in the x axis 
	float fSpawnX = fBoundLowerX + static_cast<float>(rand()) / static_cast<float>((RAND_MAX / (fBoundUpperX - fBoundLowerX))); // generate random initial postion for the explosions withn the bounds of the screen
	// generate initial target y for the power up to move to
	float fTargetY = m_shape.getSize().y * 2 + static_cast<float>(rand()) / static_cast<float>((RAND_MAX / ((m_windowHeight - m_shape.getSize().y * 2) - m_shape.getSize().y * 2)));
	m_shape.setPosition(fSpawnX, 0 - m_shape.getSize().y);// spawn location for power ups(always above the screen)

	m_target = sf::Vector2f(fSpawnX, fTargetY); //initial target for movement

}

void powerUp::applyEffect(Ball* ball,float fDt) {

	switch (m_id) // control what effect the power up applies via its id that is assigned randomly to one of the enum  constants in the constructor
	{
	  case INCREASEPADDLESIZE: 
		m_paddleRef->getShapeReference()->setScale(m_powerUpScaleFactor, m_powerUpScaleFactor);
		m_paddleRef->getShapeReference()->setFillColor(m_colour); // set the colour to the colour of the power up
		break;
	  
	  case INVERTVELOCITY:
		ball->setVelocity(-ball->getVelocity()); // invert the velocity of the ball 
		break;
	  
	  case INCREASEBALLSPEED:
		ball->setSpeed(ball->getSpeed() + fDt * m_ballSpeedMultiplier); // increase speed of the ball be fDt * m_ballSpeedMultipler
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
			
			m_paddleRef->getShapeReference()->setScale(1.0f,1.0f); // decrease scale of paddle assigned to power up if it has the increase size effect
			m_paddleRef->getShapeReference()->setFillColor(sf::Color::White);//set the colour of the paddle assigned to the power up back to white 
			break;
		
		case INCREASEBALLSPEED:  
			ball->getShapeReference()->setFillColor(sf::Color::White);// change the colour of the ball back to white 
			break;
		}

		return true; // return true as the effect has been negated and we have hit our total effect time
	}
	return false; // otherwise return false as we havent hit our total effect time
}

// used to get a refernce to the paddle the effect needs and set the direction for where the ball was going 
// at the time of collision 
void powerUp::setPaddle(Paddle* paddleReference,int iDirection) { 
	m_paddleRef = paddleReference;
	m_direction = iDirection;
}

void powerUp::setTotalEffectTime(sf::Time newEffectTime) { // used to set the duration of the power up when needed 
	m_totalEffectTime = newEffectTime;
}

sf::Vector2f  powerUp:: randomTarget() { // will assign a random target to the power up for it to move to each time it hits its current target
 
 // the following code will generate a random postion for the power up  to move to ensuring that it remains 
 // inside of the screen while doing so
  float fBoundLowerY = m_shape.getSize().y ; 
  float fBoundLowerX = m_windowWidth/4;

  float fTargetY = fBoundLowerY * 2 + static_cast<float>(rand()) / static_cast<float>((RAND_MAX / (m_windowHeight - fBoundLowerY)));
  float fTargetX = fBoundLowerX + static_cast<float>(rand()) / static_cast<float>((RAND_MAX / ((m_windowWidth - fBoundLowerX) - fBoundLowerX)));

  return sf::Vector2f(fTargetX, fTargetY); // return the new target


}

void  powerUp::setEffectTarget() { // used to determine what object the power up effect should apply to

	switch (m_id) {

	case INCREASEPADDLESIZE:
		m_hasPaddleEffect = true;
		break;
	case INVERTVELOCITY:
	case INCREASEBALLSPEED:
		m_hasBallEffect = true;
		break;

	}

}

void powerUp::drawPowerUp(sf::RenderWindow& window) { 

	window.draw(m_shape); // draw the power ups to the screen
}
  
bool powerUp::collision(sf::FloatRect fBounds) {
	// used to check collsion between power ups and the ball
	if (m_shape.getGlobalBounds().intersects(fBounds)) { 
		return true;
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

	m_currentEffectTime-=m_totalEffectTime * m_durationScalar;  /// we decrease the current effect time by the total effect time multiplied by our power up duration scalar

}
sf::Text *powerUp::getPopUpTextRef() {
	return &m_powerUpText;
}
sf::Text powerUp::getPopUpTextValue() {
	return m_powerUpText;
}
bool powerUp::hasBall() {
	return m_hasBallEffect;
}
int powerUp::getEffectDirection() {
	return m_direction;
}
sf::RectangleShape powerUp::getShape() {
	return m_shape;
}


void powerUp::setType(sf::Color colour,sf::Time effectTime,std::string powerUpText) { 
	
	// set the type of the power up i.e its effect time, colour, and pop up text 
	m_shape.setFillColor(colour);
	m_colour = colour;
	m_powerUpText.setString(powerUpText);
	m_powerUpText.setFillColor(colour);
	m_totalEffectTime = effectTime;
}

int powerUp::getId() { // get id of the power up
	return m_id;
}
void powerUp::updatePowerUpPos(float fDt) { // used to update the power up and move it around the screen

	sf::Vector2f fDistanceToTarget = m_target - m_shape.getPosition();// get distance between power up and current target
	float fLength = sqrt(fDistanceToTarget.x * fDistanceToTarget.x + fDistanceToTarget.y * fDistanceToTarget.y);// get lenght of distance
	
	fDistanceToTarget.x /= fLength; // normalise the distance vector so we have a direction
	fDistanceToTarget.y /= fLength;

	if (!m_shape.getGlobalBounds().contains(m_target)) { // if we have not  reached our target
		m_shape.move(fDistanceToTarget*(m_speed*fDt));// move along the normalised direction vector

	}
	else { // otheriwse we have hit our target
		m_targetHitCount++; // increment our current target hit count used to manage how long the power up stays on screen
		m_target = randomTarget(); // assigned a new random target within a specifc bound range 
	}
	
}


