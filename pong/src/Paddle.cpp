#include "Paddle.h"
#include <iostream>
Paddle::Paddle(sf::Vector2f position, float width, float height, sf::Color color) // constructor class for the paddle that will take in all the values that will be assging to an object of this class when it is initialised  
{
	m_size.x = width; // setting thre x attribute of the size vector which in itself is an object of the sfml vector2f class 
	m_size.y = height;
	// setting the properties of the sf::rectangle object associated with the m_shape attribute of the paddle 
	m_lastScoreCheckPlayer = 0;
	m_lastScoreCheckAi = 0;
	m_aISpeedMultiplier = 2.65f; // speed multipler for the  paddle ai to move it based on its distance to its target this variable will also be modfied based on score
	m_baseAiSpeedMultiplier = 2.65f; // used to reset ai speed
	m_aiSpeedController = 1.0f; // speed increment/decrement
	m_minAiSpeed = 2.35f; // lowest speed ai can reach
	m_maxAiSpeed = 2.85f; // maximum speed for ai 
	m_shape.setSize(m_size); // set the local size of the paddle
	m_shape.setPosition(position); // setting intitial pos
	m_shape.setFillColor(color); 
	m_shape.setOrigin(m_shape.getSize() / 2.f);
}



void Paddle::draw(sf::RenderWindow& window)
{
	window.draw(m_shape); //draw the paddle to the screen
	
}


void Paddle::move(float fDt, float fWindowYVal)
{

	checkBounds(fWindowYVal); /// keep the paddle in bounds of the screen

    m_shape.move(0, m_speed * fDt); // move the paddle 
	

}

void Paddle::checkBounds(float fWindowYVal) {
	// modified condtions of the out of bounds detection to ensure that the player and the ai paddle can no longer go off screen or get stuck at the bottom of the screen 
	float fapplyScaling = (m_shape.getSize().y / 2) * m_shape.getScale().y;
	if (m_shape.getPosition().y - fapplyScaling <= 0)  // check if the paddle would be off the top of the screen according to its current postion 
	{
		m_shape.setPosition(m_shape.getPosition().x, 0 +  fapplyScaling); // reset the paddles postion to the top of the screen+ half the paddle width keeping the paddle on screen and pushing them away from the screen edge 
	}
	else if (m_shape.getPosition().y + fapplyScaling >= fWindowYVal)  // check if the paddle would be off the bottom of the screen according to its current postion 
	{
		m_shape.setPosition(m_shape.getPosition().x, fWindowYVal - fapplyScaling);// reset the paddles postion to the bottom y coordinate  of the screen - half the paddle width keeping the paddle on screen and pushing them away from the screen edge 
	}
}

void Paddle::trackBall(sf::Vector2f fBallPos ,sf::Vector2f fBallVelocity,float fDt, float fWindowYVal) {

	
	if (fBallVelocity.x < 0.0f) { // if the ball isnt coming towards the ai we reset its wait time back to 0 and do not continue further 
	   
		m_speed = 250.0f;// reset speed while moving back to centre
		AiMovement(sf::Vector2f(m_shape.getPosition().x,fWindowYVal/2 ),fWindowYVal,fDt,fBallVelocity);// move back to centre with some offset
		return; // return while velocity is below 0 as the ai doesnt need to move from its reset position
	}

	
	sf::Vector2f fPaddlePathTop(m_shape.getPosition().x, 0); // the top of the line is at y 0 
	sf::Vector2f fPaddlePathBottom(m_shape.getPosition().x, fWindowYVal); // bottom of line is at the height of the window 
	
	// we want to get an intersection point between the ray we are casting in the getIntersectionValueFuntcion from the ball and the path the paddle has available to it on the y 
	float fIsIntersection = getRayIntersectionValue(fBallVelocity, fBallPos, fWindowYVal, fPaddlePathTop, fPaddlePathBottom);
	if (fIsIntersection != 0 ) { // if we have an interesection between the ray being cast from the ball and the paddle path
			m_aITarget = lerpToIntersection(fPaddlePathTop, fPaddlePathBottom, fIsIntersection,fWindowYVal); // we interpolate along the screen edge line to the intersection point between the ray and the line 
			
			sf::Vector2f fDistanceToTarget = m_aITarget - m_shape.getPosition();
			m_speed = abs(fDistanceToTarget.y) * (m_aISpeedMultiplier); // the AI speed is based on the distance to the target making it move fast or slower depending on how wide the target is
		
			AiMovement(m_aITarget, fWindowYVal, fDt, fBallPos); // move the ai towards the intersection point 
	}
	
	

}



void Paddle::aiValidateScore(float iPlayerScore, float iAiScore,float iMaxScore) {

	
	/// used to control the ai speed based on the percentage of how close it is to the max score and how close the player is 
	float fPlayerPercent = iPlayerScore / iMaxScore;
	float fAiPercent =  iAiScore / iMaxScore;
	if (fPlayerPercent >= 0.75f || fPlayerPercent == fAiPercent) { // when the player reaches a score close to the winning or equals out the score
		m_aISpeedMultiplier = m_baseAiSpeedMultiplier; // keep the ai speed at a consistent rate as to ensure that the player has a challenge still if they started out on the back foot and are now close to winning or equal
		return; // return as we dont modify the speed
	}
	std::cout << fPlayerPercent << std::endl;
	std::cout << fAiPercent << std::endl;
	// if the player score is greater than when the last check took place and the percentage for the ai is not higher than the player
	if (iPlayerScore > m_lastScoreCheckPlayer  &&!(m_aISpeedMultiplier >= m_maxAiSpeed || fAiPercent>=fPlayerPercent)) { 
		std::cout << "increment" << std::endl;

		m_aISpeedMultiplier += m_aiSpeedController * fPlayerPercent; // increase the ai speed by the ai speed increment multipled by how close the player is to the socre

	}
	// if we havent hit our minmum speed and the player score is smaller than the ai score and the player score percentage isnt greater than or equal to the ai
	else if (iAiScore > m_lastScoreCheckAi && !(m_aISpeedMultiplier <= m_minAiSpeed || fPlayerPercent>=fAiPercent)) {
		std::cout << "decrement" << std::endl;
		m_aISpeedMultiplier -= m_aiSpeedController * fAiPercent; // decrmeent the score by the ai speed increment multipled by how close the ai is to max score
	}
	std::cout << m_aISpeedMultiplier << std::endl;
	// reassign  new scores
	m_lastScoreCheckPlayer = iPlayerScore;
	m_lastScoreCheckAi = iAiScore;

}


void Paddle::AiMovement(sf::Vector2f fTargetposition,float fWindowYVal,float fDt,sf::Vector2f fBallPosition) {
	
	
	sf::Vector2f fVectorbetween = fTargetposition - m_shape.getPosition(); // get the vector between the current paddle postion and the target we got from getting the intersection between the ball ray and the paddles path

	
	float fDirectionMag = sqrt(fVectorbetween.x * fVectorbetween.x + fVectorbetween.y * fVectorbetween.y);

	fVectorbetween.y /= fDirectionMag; // get the direction the paddle needs to move in by normalizing the vector between the two points

	// ensure that the ai doesnt go out of bounds(similar to the player)
	 checkBounds(fWindowYVal);
 

	// moving the ai along the normalized direction vector 
	 
	float fOffsetY = fVectorbetween.y * (m_speed * fDt);
	if (m_shape.getPosition() != fTargetposition) { // if we havent reached are target intersection point 
		
		m_shape.move(0, fOffsetY);
		    
	}
	

}



// the method that is used to calculate the intersection point between the ai paddle bath and the ball
float Paddle::getRayIntersectionValue(sf::Vector2f fBallVelocity, sf::Vector2f fBallPos, float fWindowYVal, sf::Vector2f fPathTop, sf::Vector2f fPathbottom) {
	
	
	 float fVelocitymag = sqrt(fBallVelocity.x * fBallVelocity.x + fBallVelocity.y * fBallVelocity.y); // used to normalize the velocity in order to get the balls current velocity normal 
	 fBallVelocity.x /= fVelocitymag; // normaliing each compoenent of the velocity vector 
     fBallVelocity.y /= fVelocitymag;

	
	sf::Vector2f fDirection(fBallVelocity.x * 10.0f, fBallVelocity.y * 10.0f); // cast a ray in the dircection the ball is going from the balls centre point  always keeping it ahead of the ball 
	sf::Vector2f fEnd = fBallPos + fDirection;

	sf::Vector2f fPaddleLinedistance = fPathbottom - fPathTop; // this defines our line to represent the paddles path i.e  the vector from the top of the screen to the bottom of the screen using the paddles x coordinate to postion the line in accordance with the paddle  
	sf::Vector2f fRaydistance = fEnd - fBallPos; // get the distance between the end of the ray and the start(a segement of the ray) 

	float fDistanceCross = fRaydistance.x * fPaddleLinedistance.y - fRaydistance.y * fPaddleLinedistance.x; // here we take a cross product of those two distances in order to produce an overall scalar value from the two differnce vectors 

	float fRayIntersectionU = ((fPathTop.x - fBallPos.x) * fRaydistance.y - (fPathTop.y - fBallPos.y) * fRaydistance.x) / fDistanceCross; // used to compute the rate we move along the line from the top edge of the screen to the bottom egde of the screen 
	
	//  this frayIntersectionU  has been calculated such that we can find the intersection point of the ray and the line 
	// by interpolating along the line that represents the paddles movement path using the percentage rate u we will eventually reach a point where the interpolation will meet the ray representing our intersection
	
	return fRayIntersectionU; // return the percentage that will be passed into the lerp function to get the intersection point  
		
	
}

sf::Vector2f Paddle::lerpToIntersection(sf::Vector2f fStart, sf::Vector2f fEnd, float fPercent,float fWindowYVal) // used to get the intersection point between the ray being cast from the ball and the edge of the screen 
{
	  
	sf::Vector2f fInterpolation = fStart + (fEnd - fStart) * fPercent; // get the new intersection coordinate by interpolating from our starting point(our paddle path top) to a point along the paddle path based on a specifc percentage 
	
	
	if (fInterpolation.y > fWindowYVal -1 ) { // if the intersection point is off the top of the screen
		fInterpolation.y = fWindowYVal - m_shape.getSize().y/2; // we bring the y up  to a point that is reachable by the paddle
	}
	else if (fInterpolation.y < 1.0f) { // if its off the bottom of the screen
		fInterpolation.y = 0 + m_shape.getSize().y/2; // we bring the y value down to a point where the ai can reach
	}

	return fInterpolation; // return the new coordinate


}

// used to reset the paddles postion when switching game states for example when the gameOver state is triggered the paddles postion will be reset to its starting point 
void Paddle::reset(sf::Vector2f fPosition) {

	m_shape.setPosition(fPosition); // call the set postion method  related to the m_shape attribute which will overide its current postion with the postion argument of type sf::vector2f
	m_baseAiSpeedMultiplier = m_baseAiSpeedMultiplier; // reset the ai speed 

}
// getters  for the paddle attributes that will return the values of the private attributes for the paddle that would otherwise be inacessible 
//  below also has setters for paddle private attributes allowing for manipulation of their values  
sf::FloatRect Paddle::getBounds() const
{
	return m_shape.getGlobalBounds();
}


sf::RectangleShape Paddle::getShape()
{
	return m_shape;
}
sf::RectangleShape* Paddle::getShapeReference() {
	return &m_shape;
}

void Paddle::setSpeed(float fSpeed)
{
	m_speed = fSpeed;
}
