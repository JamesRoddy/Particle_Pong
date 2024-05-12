#include "Paddle.h"

Paddle::Paddle(sf::Vector2f position, float width, float height, sf::Color color)
{
	m_size.x = width;
	m_size.y = height;
	m_shape.setSize(m_size);
	m_shape.setPosition(position);
	m_shape.setFillColor(color);
	m_shape.setOrigin(m_shape.getSize() / 2.f);
}

void Paddle:: reset(sf::Vector2f position) {

	m_shape.setPosition(position);


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



void Paddle::trackBall(sf::Vector2f ballPos,float dt,float windowYVal) { 

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
