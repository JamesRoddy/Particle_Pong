#include "Ball.h"
#include<iostream>
Ball::Ball(sf::Vector2f position, float radius, float speed, sf::Color color)
{
	m_speed = speed;
	m_velocity.x = speed;
	m_velocity.y = speed;
	m_speedIncreaseMultiplier = 20;
	m_maxSpeed = 500.0f;
	m_shape.setRadius(radius);
	m_shape.setPosition(position);
	m_shape.setFillColor(color);
	m_shape.setOrigin(m_shape.getRadius() / 2, m_shape.getRadius() / 2);
}

void Ball::draw(sf::RenderWindow& window)
{
	window.draw(m_shape);
}
void Ball::increaseSpeed(float dt) { // allow for maniplation of the ball speed(setter for ball speed) through calling this method

	float differnce = m_maxSpeed - m_speed; // get the differnce between the current speed and the maximum speed 
	
	if ( differnce > (dt*m_speedIncreaseMultiplier)) {  /// if the differnce is greater than the current delta time multipled by the speed increase multipler 
		m_speed += dt*m_speedIncreaseMultiplier; // increase the speed using a multipler otherwise the speed increase wouldnt be noticable during a game due to the value of DT being realtively small 
		std::cout << m_speed << std::endl;
	
	}
	else {
		m_speed = m_maxSpeed;
	}

}
void Ball::move(float dt, sf::RenderWindow& window)
{
	

	if (m_shape.getPosition().y < 1 ||
		m_shape.getPosition().y + m_shape.getRadius() * 2 > window.getSize().y - 1)
		m_velocity.y = -m_velocity.y;
	m_shape.move(m_velocity * dt);
}



sf::Vector2f Ball::getPosition()
{
	return m_shape.getPosition();
}

void Ball::setPosition(float x, float y)
{
	m_shape.setPosition(x, y);
}

void Ball::updateVelocity(float val)
{
	m_velocity.x = m_speed * val;
}
void Ball::resetPos(float newDirection, int newX, int newY) {
	setPosition(newX, newY); // set the postion of the ball to the centre of the screen once the ball has passed the paddle(half the screen width and half the screen height)
	m_speed = 300.0f;// give the ball a slower speed to give the player more time to react when it resets
	updateVelocity(newDirection); // make the ball go in the opposite direction to the side it just scored on 
}
sf::CircleShape Ball::getShape()
{
	return m_shape;
}




