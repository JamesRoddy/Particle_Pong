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

void Paddle::draw(sf::RenderWindow& window)
{
	window.draw(m_shape);
}

void Paddle::move(float dt, float windowYVal)
{
	std::cout << windowYVal << std::endl;
	if (m_shape.getPosition().y - m_shape.getSize().y / 2 > windowYVal)
		m_shape.move(0, -m_speed * dt);

	if (m_shape.getPosition().y + m_shape.getSize().y / 2 < windowYVal)
		m_shape.move(0, m_speed * dt);
	


}



void Paddle::trackBall(sf::Vector2f ballPos,float dt,float windowYVal) { 

	sf::Vector2f forward = ballPos - m_shape.getPosition();// get vector between the ballPos and the current position of the paddle 
	
	if ( forward.y<=0 ) {// if the differnce is neagtive(meaning that the paddle is above the ball)

		if (m_shape.getPosition().y <= m_size.y / 2) { // we first check if the paddle is at the top right of the screen(preventing it from going out of bounds
			setSpeed(0.0f); // set speed to zero if paddle would go of screen 
		}
		else { // otherwise we allow the paddle to move upwards towards the ball 
			setSpeed(375.0f);
			move(-dt, windowYVal);
		}
	

	}
	else{ // else the difference in y is postive meaning the ballPos is below the paddle 
		if (m_shape.getPosition().y >= (windowYVal - m_size.y/2)-10 ) { // check if the paddle would go out of bounds with a slight offset to esure that the paddle doesn't get completly stuck(ensuring its not directly centre as the paddle wouldnt be able to escape)
			setSpeed(0.0f); // set speed to 0 if true 
		}
		else { // otherwise allow the paddle to track the ball and move down to intercept 
			setSpeed(375.0f);
			move(dt, windowYVal);
		}
		

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
