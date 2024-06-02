
#pragma once
#include "Ball.h"

#include<iostream>

Ball::Ball(sf::Vector2f position, float radius, float speed, sf::Color color)
{
	// assigning the attributes to any instance/object created that is of the ball class
	//  all of the arguments passed into the constructor are assgined to the attributes when it is called 
	m_speed = speed;
	m_velocity.x = speed;
	m_velocity.y = speed;
	m_speedIncreaseMultiplier = 10.0f; // used to increase ball speed each collision
	maxCollsionAngle = 45.0f; // maximum collsion angle for ball
	m_maxSpeed = 575.0f; // maximum speed ball can reach
	m_shape.setRadius(radius); // set the radius of the m_shape attribute that repsents an object of the inbuilt CircleShape class wihtin sfml this wil ajsut the size of the ball using the float raidus argument passed into the constructor for the ball class 
	m_shape.setPosition(position);  // set the initial postion of the ball from inside this constructor
	m_shape.setFillColor(color);
	m_shape.setOrigin(m_shape.getRadius() / 2, m_shape.getRadius() / 2);
}

void Ball::draw(sf::RenderWindow& window)
{
	window.draw(m_shape);

}
void Ball::increaseSpeed(float fDt) { // increase ball speed each collision time through calling this method

	if (!(m_speed>m_maxSpeed)) { // if we havent reached our max velocity
			m_speed += fDt * m_speedIncreaseMultiplier; // increase the speed using a multipler otherwise the speed increase wouldnt be noticable during a game due to the value of DT being realtively small 
		
	
	}

	

}
void Ball::move(float fDt, sf::RenderWindow& window) // this method is used to move the ball based on its current speed and velocity
{

	// adjusting the bounds checking of the ball to better avoid it getting stuck at the top of the screen by taking into accoutn the radius of the ball for both checks 
	if (m_shape.getPosition().y - m_shape.getRadius() <= 0) {

		m_shape.setPosition(m_shape.getPosition().x, 0 + m_shape.getRadius()); // ensure to move the ball back by half before inverting the velocity 
		m_velocity.y = -m_velocity.y; // invert the balls velocity making it go a different direction 
	}
	else if (m_shape.getPosition().y + m_shape.getRadius()  >= window.getSize().y) {

		m_shape.setPosition(m_shape.getPosition().x, window.getSize().y - m_shape.getRadius());
		m_velocity.y = -m_velocity.y; // invert the balls velocity making it go a different y direction 
	}
	// both condtions above are used to change the direction of the ball each time it hits the top edge or bottm edge of the screen  in order to create a bouncing effect
	
	m_shape.move(m_velocity * fDt); // call the move method associtated with the CircleShape class that the m_shape attribute is an object of which will move the shape by an offest of the velocity attribute(an sf::vector2f object) multiplied by delta time(an argument passed into this method) 
}



sf::Vector2f Ball::getPosition()
{
	return m_shape.getPosition(); // return the resul of calling the get postion method on the m_shape attribute contained within the ball class 
	// this wil return an sfml vector2f object representing the current coordinates of the m_shape object  
}



bool Ball::ballCollisionPushBack(sf::RectangleShape paddleBounds,float dt) {


	// this code is a rectangle to circle intersection algorithm and has been implmented for better accuracy of collsion 
	// rather than just using the two bounding boxes of the CircleShape and the rectangle sfml provides 
	// in order to generate the collsion particles when the ball collides with the paddle 
	sf::Vector2f fDistanceBetweenBall = paddleBounds.getPosition() - m_shape.getPosition(); // get the vector between the current paddle position and the ball

	float fMinX = paddleBounds.getGlobalBounds().left; //get the minmum top left cordinate of the paddle we are checking on the x axis 
	float fMinY = paddleBounds.getGlobalBounds().top; // get the minumum top left cordinate of the paddle we are checking on the fMinY axis 
	
	// get the minimum value between the balls psotion and the maximum x cordinate the paddle covers ensuring that the surfacePoint of collision stays within the bounds 
	// of the paddle on the x axis then max that with the minimumX cordinate of the paddle this means that if the ball isnt within the paddle we will always get the postion of the surface point at minimum 
	// or maximum X depending on if the ball is on the left or right of the paddle(same for fMinY)
	float fsurfacePointX = std::max(fMinX,std::min(m_shape.getPosition().x, fMinX + paddleBounds.getGlobalBounds().width));  
	float fsurfacePointY = std::max(fMinY, std::min(m_shape.getPosition().y, fMinY + paddleBounds.getGlobalBounds().height));
	
	sf::Vector2f fNearestPointToSurface = sf::Vector2f(fsurfacePointX - m_shape.getPosition().x, fsurfacePointY - m_shape.getPosition().y); // get the differnce between the surfacePoint of collsion on the paddle and the balls actaul postion
	float fdistanceToSurfacePoint = sqrt(powf(fNearestPointToSurface.x, 2) + powf(fNearestPointToSurface.y, 2)); // get the maginutude of the vector between the balls postion represented in the bounds of the paddle and the balls actual postion 
	
	if (fdistanceToSurfacePoint < m_shape.getRadius()) { // if the distance to the surface point x and y is smaller than the balls current radius meanig that we have an fOverlap 
		
		float fOverlap = m_shape.getRadius() - fdistanceToSurfacePoint; // calculate  the amount the ball overlapped which is the differnce bewteen the balls radius and the distance between the surface point as the ball is currenlty overlapping the paddle 
		sf::Vector2f fNormaliseCollisionVector = sf::Vector2f(fNearestPointToSurface.x / fdistanceToSurfacePoint, fNearestPointToSurface.y / fdistanceToSurfacePoint);
		// get the collsion normal which is the vector between the centre of the circle and surface point normalsied 
		
		// move the ball along the direction of inverted collsion normal so that it travels backward 
		m_shape.move(-fNormaliseCollisionVector * (m_shape.getRadius()*2 + fOverlap)); // push the ball back by its full diameter plus the overlap we calculated correcting the collision 
		float fDistanceToCentre = fsurfacePointY - paddleBounds.getPosition().y; // get distance between surface point where collsion happend and the centre of paddle

		float fRotationY = getRotation(fDistanceToCentre, paddleBounds);
	
	    m_velocity.y = fRotationY * m_speed;  // apply rotation to velocity vector y
	
	    if (fDistanceBetweenBall.x<0) { // if the distance between the ball is less then 0 then the ball is coming from the right as the paddles postion will be smaller than the ball 
		    updateVelocity(1); // so we update the x velcoity to be positive making the ball move to the right again 
		
		}
		else {
	   	    updateVelocity(-1); // opposite of above 
		}
		return true; // collsion happend 
	 
	}

    return false; // collsion didn't happen


	

}

float Ball::getRotation(float fDistanceToCentre, sf::RectangleShape paddleBounds) {
	// the following code will give a spin/rotation to the balls fMinY velocity depending on where it hits the paddle
	// allowing for more steep and vaired angles the ball can take after hitting the paddle 
	float fRelativeDistanceToCentre = fDistanceToCentre * 2 / paddleBounds.getGlobalBounds().height; // get the surface point distance to the centre of the paddle as a percentage

	float angle = fRelativeDistanceToCentre * maxCollsionAngle; // adjust the angle based on the percentage point caluclated(how close collsion point is to centre)
	float fRotationY = fRotationY = sin(angle) + cos(angle); // calculate rotation that should be applied to the current velocity vector fMinY; 
	if (fRelativeDistanceToCentre < 0) { // base the sign of the rotation of where it hit the paddle
		fRotationY = -(abs(fRotationY));
	}
	else if (fRelativeDistanceToCentre > 0) {
		fRotationY = abs(fRotationY);
	}
	return fRotationY;
}

// this mehtod will set the psotion of the m_shape object( assgine dot the built in CircleShape class) based on the passed in float arguments(x,fMinY)
void Ball::setPosition(float fX, float fY)
{
	m_shape.setPosition(fX, fY); 
}


void Ball::updateVelocity(float fVal)
{
	if (m_velocity.y == 0) { // if the ball has recently reset(y vel is set to 0 when this happens) and a collsion has occurred meaning we need to invert the velocity
		m_velocity.y = m_speed; // set the y velocity back to the current speed 
	}
	m_velocity.x = m_speed * fVal;
}
void Ball::resetPos(int iNewX, int iNewY ) {

	m_velocity.y = 0; // set y velocity at 0 allowing for each serve to be in a straight line on the x axis 
	m_speed = 400.0f;
	m_velocity.x = -m_speed;// give the ball a slower speed to give the player more time to react when it resets
	
	setPosition(iNewX, iNewY); // set the postion of the ball to the centre of the screen once the ball has passed the paddle(half the screen width and half the screen height)
	
}




// getters and setters for the balls private properties 

void Ball::setVelocity(sf::Vector2f fNewVelocity) {

	m_velocity = fNewVelocity;
}
float Ball::getSpeed() {
	return m_speed;
}

void Ball::setSpeed(float fNewSpeedValue) {
	m_speed = fNewSpeedValue;
}

sf::CircleShape* Ball::getShapeReference() { // get a refernce address to the ball object(used by power ups)

	return &m_shape;
}
sf::Vector2f Ball::getVelocity() {
	return m_velocity;
}
// allows access to the private m_shape attribute that is conatined wihtin the ball class(and assgine to any object of the ball class)
sf::CircleShape Ball::getShape()
{
	return m_shape; // this will allow access to all of the methods asscoiated with the m_shape attribute
}




