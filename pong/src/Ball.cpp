#include "Ball.h"
#include<iostream>
Ball::Ball(sf::Vector2f position, float radius, float speed, sf::Color color)
{
	// asiging the attributes to any instance/object created that is of the ball class
	//  all of the arguments passed into the constructor are assgined to the attributes when it is called 
	m_speed = speed;
	m_velocity.x = speed;
	m_velocity.y = speed;
	m_speedIncreaseMultiplier = 30;
	m_maxSpeed = 600.0f;
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
	
	if ( differnce > (dt * m_speedIncreaseMultiplier)) {  /// if the differnce is greater than the current delta time multipled by the speed increase multipler 
		m_speed += dt*m_speedIncreaseMultiplier; // increase the speed using a multipler otherwise the speed increase wouldnt be noticable during a game due to the value of DT being realtively small 
		 
	     
	}
	else {
		m_speed = m_maxSpeed; // otherwise if the max speed is hit we ensure that it doesnt go over that value 
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
	return m_shape.getPosition(); // return the resul of calling the get postion method on the m_shape attribute contained within the ball class 
	// this wil return an sfml vector2f object repsenting the current coordinates of the m_shape object  
}

// this mehtod will set the psotion of the m_shape object( assgine dot the built in CircleShape class) based on the passed in float arguments(x,y)
void Ball::setPosition(float x, float y)
{
	m_shape.setPosition(x, y); 
}

void Ball::updateVelocity(float val)
{
	if (m_velocity.y == 0) { // if the ball has recently reset(y vel is set to 0 when this happens) and a collsion has occurred meaning we need to invert the velocity
		m_velocity.y = m_speed; // set the y velocity back to the current speed 
	}
	m_velocity.x = m_speed * val;
}
void Ball::resetPos(float newDirection, int newX, int newY ) {

	m_velocity.y = 0; // set y velocity at 0 allowing for each serve to be in a straight line on the x axis 
	m_speed = 300.0f;
	m_velocity.x = m_speed;// give the ball a slower speed to give the player more time to react when it resets
	
	setPosition(newX, newY); // set the postion of the ball to the centre of the screen once the ball has passed the paddle(half the screen width and half the screen height)
	
}

// allows access to the private m_shape attribute that is conatined wihtin the ball class(and assgine to any object of the ball class)
sf::CircleShape Ball::getShape()
{
	return m_shape; // this will allow access to all of the methods asscoiated with the m_shape attribute
}




