#include "Paddle.h"
#include <iostream>
Paddle::Paddle(sf::Vector2f position, float width, float height, sf::Color color) // constructor class for the paddle that will take in all the values that will be assging to an object of this class when it is initialised  
{
	m_size.x = width; // setting thre x attribute of the size vector which in itself is an object of the sfml vector2f class 
	m_size.y = height;
	
	// setting the properties of the sf::rectangle object associated with the m_shape attribute of the paddle 
	m_debug0.setRadius(5.0f);
	m_debug0.setFillColor(sf::Color::Blue);
	m_debug1.setRadius(5.0f);
	m_debug1.setFillColor(sf::Color::Red);
	m_debug2.setRadius(5.0f);
	m_debug2.setFillColor(sf::Color::Red);
	m_shape.setSize(m_size); 
	m_shape.setPosition(position);
	m_shape.setFillColor(color);
	m_shape.setOrigin(m_shape.getSize() / 2.f);
}


// used to reset the paddles postion when switching game states for example when the gameOver state is triggered the paddles postion will be reset to its starting point 
void Paddle::reset(sf::Vector2f position) {

	m_shape.setPosition(position); // call the set postion method  related to the m_shape attribute which will overide its current postion with the postion argument of type sf::vector2f


}
void Paddle::draw(sf::RenderWindow& window)
{
	window.draw(m_shape);
	window.draw(m_debug0);
	window.draw(m_debug1);
	window.draw(m_debug2);
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






void Paddle::trackBall(sf::Vector2f ballPos,sf::Vector2f velocity,float dt,float windowYVal,float fwindowXVal) { 
	
	
	// we want to get an intersection point between the ray we are casting in the getIntersectionValueFuntcion from the ball and the path the paddle has available to it on the y 
	sf::Vector2f fpaddlePathTop(m_shape.getPosition().x, 0); // the top of the line is at y 0 
	sf::Vector2f fpaddlePathBottom(m_shape.getPosition().x, windowYVal); // bottom of line is at the height of the window 
	
	float isIntersection = getRayIntersectionValue(velocity, ballPos, windowYVal,fpaddlePathTop,fpaddlePathBottom); 

	if (isIntersection != 0) {
		
		sf::Vector2f fintersectionPoint = lerpToIntersection(fpaddlePathTop, fpaddlePathBottom, isIntersection);
		AiMovement(fintersectionPoint, windowYVal,dt);

	}


	  
	

	//sf::Vector2f distance = ballPos - m_shape.getPosition();// get vector between the ballPos and the current position of the paddle 
	//
	//
	//if ( distance.y<=0 ) {// if the differnce is neagtive(meaning that the paddle is above the ball)

	//	// we allow the paddle to move upwards towards the ball 
	//		move(-dt, windowYVal);
	//}
	//else{ // else the difference in y is postive meaning the ballPos is below the paddle 	
	//////  allow the paddle to track the ball and move down to intercept 
	//   move(dt, windowYVal);
	//
	//}

}



void Paddle::AiMovement(sf::Vector2f targetVector,float windowYVal,float dt) {

	sf::Vector2f fvectorBetween = targetVector - m_shape.getPosition(); // get the vector between the current paddle postion and the target we got from getting the intersection between the ball ray and the paddles path

	float fdirectionMag = sqrt(fvectorBetween.x * fvectorBetween.x + fvectorBetween.y * fvectorBetween.y);

	float fnormalizedY = fvectorBetween.y /= fdirectionMag; // get the direction the paddle needs to move in by normalizing the vector between the two points

	// modified condtions of the out of bounds detection to ensure that the player and the ai paddle can no longer go off screen or get stuck at the bottom of the screen 
	if (m_shape.getPosition().y - m_shape.getSize().y / 2 <= 0)  // check if the paddle would be off the top of the screen according to its current postion 
	{
		m_shape.setPosition(m_shape.getPosition().x, 0 + m_shape.getSize().y / 2); // reset the paddles postion to the top of the screen+ half the paddle width keeping the paddle on screen and pushing them away from the screen edge 
	}
	else if (m_shape.getPosition().y + m_shape.getSize().y / 2 >= windowYVal)  // check if the paddle would be off the bottom of the screen according to its current postion 
	{
		m_shape.setPosition(m_shape.getPosition().x, windowYVal - m_shape.getSize().y / 2);// reset the paddles postion to the bottom y coordinate  of the screen - half the paddle width keeping the paddle on screen and pushing them away from the screen edge 
	}

	if (m_shape.getPosition() != targetVector) { // if we havent reached are target intersection point 
		float offsetY = fnormalizedY * (m_speed * dt); // we move along the calculated direction vector by the paddles speed * dt
		m_shape.move(0, offsetY); 
	}
	
	
	




}
float Paddle::getRayIntersectionValue(sf::Vector2f velocityVec, sf::Vector2f ballPos,float windowYVal,sf::Vector2f lineTop,sf::Vector2f lineBottom) {
	
	
	float fvelocityMag = sqrt(velocityVec.x * velocityVec.x + velocityVec.y * velocityVec.y); // used to normalize the velocity in order to get the balls current velocity normal 
	float fnormalizedX = velocityVec.x / fvelocityMag; // normaliing each compoenent of the velocity vector 
	float fnormalizedY = velocityVec.y / fvelocityMag;

	sf::Vector2f forigin = ballPos; // where we will be casting the ray from 
	sf::Vector2f vfdirection(fnormalizedX * 10.0f, fnormalizedY * 10.0f); // cast a ray in the dircection the ball is going from the balls centre point  always keeping it ahead of the ball 
	sf::Vector2f fend = forigin + vfdirection;

	sf::Vector2f fpaddleLineDistance = lineBottom - lineTop; // this defines our line to represent the paddles path i.e  the vector from the top of the screen to the bottom of the screen using the paddles x coordinate to postion the line in accordance with the paddle  
	sf::Vector2f frayDistance = fend - forigin; // get the distance between the end of the ray and the start(a segement of the ray) 

	float fdistanceCross = frayDistance.x * fpaddleLineDistance.y - frayDistance.y * fpaddleLineDistance.x; // here we take a cross product of those two distances in order to produce an overall scalar value from the two differnce vectors 

	float frayIntersectionU = ((lineTop.x - forigin.x) * frayDistance.y - (lineTop.y - forigin.y) * frayDistance.x) / fdistanceCross; // used to compute the rate we move along the line from the top edge of the screen to the bottom egde of the screen 
	
	// these two rates have been calculated such that we can find the intersection point of the ray and the line 
	// by interpolating along the line using the u rate and along the ray using the the t rate eventually the interpolated points will meet

	//if (frayIntersectionT >= 0 && frayIntersectionT <= 1 && frayIntersectionU >= 0 && frayIntersectionU <= 1) { // if both u and t are in the range 0 to 1 meaning that we have an intersection that is in the range of the line sgement we are checking  the ray against 
		m_debug2.setPosition(lerpToIntersection(lineTop, lineBottom, frayIntersectionU));
		return frayIntersectionU; // we move along the line the percentage u and lerp along the line to that intersection point with the ray 
		
	//} 
	
	
}

sf::Vector2f Paddle::lerpToIntersection(sf::Vector2f start, sf::Vector2f end, float percent) // used to get the intersection point between the ray being cast from the ball and the edge of the screen 
{
	  
	sf::Vector2f finterpolation = start + (end - start) * percent;

	return finterpolation;


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



void Paddle::setSpeed(float fspeed)
{
	m_speed = fspeed;
}
