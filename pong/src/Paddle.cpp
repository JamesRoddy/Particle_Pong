#include "Paddle.h"
#include <iostream>
Paddle::Paddle(sf::Vector2f position, float width, float height, sf::Color color) // constructor class for the paddle that will take in all the values that will be assging to an object of this class when it is initialised  
{
	m_size.x = width; // setting thre x attribute of the size vector which in itself is an object of the sfml vector2f class 
	m_size.y = height;
	m_initialHeight = width;
	m_initialHeight = height;
	// setting the properties of the sf::rectangle object associated with the m_shape attribute of the paddle 
	
	m_aISpeedMultiplier = 2.65; // speed multipler for the  paddle ai to move it based on its distance to its tareget
	
	m_shape.setSize(m_size); 
	m_shape.setPosition(position);
	m_shape.setFillColor(color);
	m_shape.setOrigin(m_shape.getSize() / 2.f);
}



void Paddle::draw(sf::RenderWindow& window)
{
	window.draw(m_shape); //draw the paddle to the screen
	
}

void Paddle::move(float dt, float windowYVal)
{

	checkBounds(windowYVal);

	
    m_shape.move(0, m_speed * dt); // move the paddle 
	

}

void Paddle::checkBounds(float windowYVal) {
	// modified condtions of the out of bounds detection to ensure that the player and the ai paddle can no longer go off screen or get stuck at the bottom of the screen 
	float fapplyScaling = (m_shape.getSize().y / 2) * m_shape.getScale().y;
	if (m_shape.getPosition().y - fapplyScaling <= 0)  // check if the paddle would be off the top of the screen according to its current postion 
	{
		m_shape.setPosition(m_shape.getPosition().x, 0 +  fapplyScaling); // reset the paddles postion to the top of the screen+ half the paddle width keeping the paddle on screen and pushing them away from the screen edge 
	}
	else if (m_shape.getPosition().y + fapplyScaling >= windowYVal)  // check if the paddle would be off the bottom of the screen according to its current postion 
	{
		m_shape.setPosition(m_shape.getPosition().x, windowYVal - fapplyScaling);// reset the paddles postion to the bottom y coordinate  of the screen - half the paddle width keeping the paddle on screen and pushing them away from the screen edge 
	}
}

void Paddle::trackBall(sf::Vector2f ballPos ,sf::Vector2f ballVelocity,float dt, float windowYVal, float windowXVal, float ballRadius) {

	
	if (ballVelocity.x < 0.0f) { // if the ball isnt coming towards the ai we reset its wait time back to 0 and do not continue further 
	    
	
		m_speed = 400.0f;
		AiMovement(sf::Vector2f(m_shape.getPosition().x,windowYVal/2 ),windowYVal,dt,ballVelocity);// move back to centre with some offset
		return;
	}

	
	sf::Vector2f fpaddlePathTop(m_shape.getPosition().x, 0); // the top of the line is at y 0 
	sf::Vector2f fpaddlePathBottom(m_shape.getPosition().x, windowYVal); // bottom of line is at the height of the window 
	
	// we want to get an intersection point between the ray we are casting in the getIntersectionValueFuntcion from the ball and the path the paddle has available to it on the y 
	float fisIntersection = getRayIntersectionValue(ballVelocity, ballPos, windowYVal, fpaddlePathTop, fpaddlePathBottom);
	if (fisIntersection != 0 ) { // if we have an interesection between the ray being cast from the ball and the paddle path
			m_aITarget = lerpToIntersection(fpaddlePathTop, fpaddlePathBottom, fisIntersection,windowYVal); // we interpolate along the screen edge line to the intersection point between the ray and the line 
			
			sf::Vector2f distanceToTarget = m_aITarget - m_shape.getPosition();
			m_speed = abs(distanceToTarget.y) * (m_aISpeedMultiplier); // the AI speed is based on the distance to the target making it move fast or slower depending on how wide the target is
		
			
			AiMovement(m_aITarget, windowYVal, dt, ballPos); // move the ai towards the intersection point 
	}
	
	

}


void Paddle::AiMovement(sf::Vector2f targetPosition,float windowYVal,float dt,sf::Vector2f ballPosition) {
	
	
	sf::Vector2f fvectorBetween = targetPosition - m_shape.getPosition(); // get the vector between the current paddle postion and the target we got from getting the intersection between the ball ray and the paddles path

	
	float fdirectionMag = sqrt(fvectorBetween.x * fvectorBetween.x + fvectorBetween.y * fvectorBetween.y);

	fvectorBetween.y /= fdirectionMag; // get the direction the paddle needs to move in by normalizing the vector between the two points

	// ensure that the ai doesnt go out of bounds(similar to the player)
	 checkBounds(windowYVal);
 

	// moving the ai along the normalized direction vector 
	 
	float offsetY = fvectorBetween.y * (m_speed * dt);
	if (m_shape.getPosition() != targetPosition) { // if we havent reached are target intersection point 
		
		m_shape.move(0, offsetY);
		    
	}
	

}



// the method that is used to calculate the intersection point between the ai paddle bath and the ball
float Paddle::getRayIntersectionValue(sf::Vector2f velocityVec, sf::Vector2f ballPos, float windowYVal, sf::Vector2f pathTop, sf::Vector2f pathBottom) {
	
	
	float fvelocityMag = sqrt(velocityVec.x * velocityVec.x + velocityVec.y * velocityVec.y); // used to normalize the velocity in order to get the balls current velocity normal 
	 velocityVec.x /= fvelocityMag; // normaliing each compoenent of the velocity vector 
     velocityVec.y /= fvelocityMag;

	
	sf::Vector2f vfdirection(velocityVec.x * 10.0f, velocityVec.y * 10.0f); // cast a ray in the dircection the ball is going from the balls centre point  always keeping it ahead of the ball 
	sf::Vector2f fend = ballPos + vfdirection;

	sf::Vector2f fpaddleLineDistance = pathBottom - pathTop; // this defines our line to represent the paddles path i.e  the vector from the top of the screen to the bottom of the screen using the paddles x coordinate to postion the line in accordance with the paddle  
	sf::Vector2f frayDistance = fend - ballPos; // get the distance between the end of the ray and the start(a segement of the ray) 

	float fdistanceCross = frayDistance.x * fpaddleLineDistance.y - frayDistance.y * fpaddleLineDistance.x; // here we take a cross product of those two distances in order to produce an overall scalar value from the two differnce vectors 

	float frayIntersectionU = ((pathTop.x - ballPos.x) * frayDistance.y - (pathTop.y - ballPos.y) * frayDistance.x) / fdistanceCross; // used to compute the rate we move along the line from the top edge of the screen to the bottom egde of the screen 
	
	//  this frayIntersectionU  has been calculated such that we can find the intersection point of the ray and the line 
	// by interpolating along the line that represents the paddles movement path using the percentage rate u we will eventually reach a point where the interpolation will meet the ray representing our intersection
	
	return frayIntersectionU; // return the percentage that will be passed into the lerp function to get the intersection point  
		
	
}

sf::Vector2f Paddle::lerpToIntersection(sf::Vector2f start, sf::Vector2f end, float percent,float windowYVal) // used to get the intersection point between the ray being cast from the ball and the edge of the screen 
{
	  
	sf::Vector2f finterpolation = start + (end - start) * percent; // get the new intersection coordinate by interpolating from our starting point(our paddle path top) to a point along the paddle path based on a specifc percentage 
	
	
	if (finterpolation.y > windowYVal -1 ) { // if the intersection point is off the top of the screen
		finterpolation.y = windowYVal - m_shape.getSize().y/2; // we bring the y up  to a point that is reachable by the paddle
	}
	else if (finterpolation.y < 1.0f) { // if its off the bottom of the screen
		finterpolation.y = 0 + m_shape.getSize().y/2; // we bring the y value down to a point where the ai can reach
	}

	return finterpolation; // return the new coordinate


}

// used to reset the paddles postion when switching game states for example when the gameOver state is triggered the paddles postion will be reset to its starting point 
void Paddle::reset(sf::Vector2f position) {

	m_shape.setPosition(position); // call the set postion method  related to the m_shape attribute which will overide its current postion with the postion argument of type sf::vector2f


}
// getters  for the paddle attributes that will return the values of the private attributes for the paddle that would otherwise be inacessible 
//  below also has setters for paddle private attributes allowing for manipulation of their values  
sf::FloatRect Paddle::getBounds() const
{
	return m_shape.getGlobalBounds();
}

sf::Vector2f Paddle::getIntialSize() {
	return sf::Vector2f(m_initialWidth, m_initialHeight);
}
sf::RectangleShape Paddle::getShape()
{
	return m_shape;
}
sf::RectangleShape* Paddle::getShapeReference() {
	return &m_shape;
}

void Paddle::setSpeed(float fspeed)
{
	m_speed = fspeed;
}
