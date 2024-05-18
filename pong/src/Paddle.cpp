#include "Paddle.h"

Paddle::Paddle(sf::Vector2f position, float width, float height, sf::Color color) // constructor class for the paddle that will take in all the values that will be assging to an object of this class when it is initialised  
{
	m_size.x = width; // setting thre x attribute of the size vector which in itself is an object of the sfml vector2f class 
	m_size.y = height;
	
	// setting the properties of the sf::rectangle object associated with the m_shape attribute of the paddle 
	m_shape.setSize(m_size); 
	m_shape.setPosition(position);
	m_shape.setFillColor(color);
	m_shape.setOrigin(m_shape.getSize() / 2.f);
}

bool Paddle::collsion(sf::CircleShape ballBounds) {
	
	
	sf::Vector2f distanceBetweenBall = (m_shape.getPosition().x - ballBounds.getPosition().x, m_shape.getPosition() - ballBounds.getPosition());
	float distanceLength = sqrt(powf(distanceBetweenBall.x, 2) + powf(distanceBetweenBall.y, 2)) - ballBounds.getRadius() - m_shape.getSize().x/2;
	
    
	if (m_shape.getGlobalBounds().contains(ballBounds.getPosition())){
		
		return true; 

		
		
	}
	
	return false;
   

}
// used to reset the paddles postion when switching game states for example when the gameOver state is triggered the paddles postion will be reset to its starting point 
void Paddle::reset(sf::Vector2f position) {

	m_shape.setPosition(position); // call the set postion method  related to the m_shape attribute which will overide its current postion with the postion argument of type sf::vector2f


}
void Paddle::draw(sf::RenderWindow& window)
{
	window.draw(m_shape);
	
}

void Paddle::move(float dt, float windowYVal)
{
	// modified condtions of the out of bounds detection to ensure that the player and the ai paddle can no longer go off screen or get stuck at the bottom of the screen 
	if (m_shape.getPosition().y - m_shape.getSize().y / 2 <= 0)  // check if the paddle would be off the top of the screen according to its current postion 
	{ 
		m_shape.setPosition(m_shape.getPosition().x, 0 + m_shape.getSize().y / 2); // reset the paddles postion to the top of the screen+ half the paddle width keeping the paddle on screen and pushing them away from the screen edge 
	}
	else if (m_shape.getPosition().y + m_shape.getSize().y / 2 >= windowYVal)  // check if the paddle would be off the bottom of the screen according to its current postion 
	{
		m_shape.setPosition(m_shape.getPosition().x,windowYVal - m_shape.getSize().y/2);// reset the paddles postion to the bottom y coordinate  of the screen - half the paddle width keeping the paddle on screen and pushing them away from the screen edge 
	}
    m_shape.move(0, m_speed * dt); // move the paddle 
	

}



void Paddle::trackBall(sf::Vector2f ballPos,sf::Vector2f playerPos,float dt,float windowYVal) { 

	sf::Vector2f distance = ballPos - m_shape.getPosition();// get vector between the ballPos and the current position of the paddle 
	
	
	if ( distance.y<=0 ) {// if the differnce is neagtive(meaning that the paddle is above the ball)

		// we allow the paddle to move upwards towards the ball 
			move(-dt, windowYVal);
	}
	else{ // else the difference in y is postive meaning the ballPos is below the paddle 	
	//  allow the paddle to track the ball and move down to intercept 
	move(dt, windowYVal);
	
	}

}
// getters for the paddle attributes that will return the values of the private attributes for the paddle that would otherwise be inacessible 
// setters for paddle private attributes 
sf::FloatRect Paddle::getBounds() const
{
	return m_shape.getGlobalBounds();
}

sf::RectangleShape Paddle::getShape()
{
	return m_shape;
}

void Paddle::setSpeed(float speed)
{
	m_speed = speed;
}
