#include "Ball.h"
#include<iostream>
#include<iostream>
Ball::Ball(sf::Vector2f position, float radius, float speed, sf::Color color)
{
	// assigning the attributes to any instance/object created that is of the ball class
	//  all of the arguments passed into the constructor are assgined to the attributes when it is called 
	m_speed = speed;
	m_velocity.x = speed;
	m_velocity.y = speed;
	m_speedIncreaseMultiplier = 30;
	
	m_maxSpeed = 600.0f;
	m_debug.setRadius(2.0f);
	m_debug.setFillColor(sf::Color::Blue);
	m_debug.setOrigin(m_debug.getRadius() / 2, m_debug.getRadius() / 2);

	m_shape.setRadius(radius); // set the radius of the m_shape attribute that repsents an object of the inbuilt CircleShape class wihtin sfml this wil ajsut the size of the ball using the float raidus argument passed into the constructor for the ball class 
	m_shape.setPosition(position); 
	m_shape.setFillColor(color);
	m_shape.setOrigin(m_shape.getRadius() / 2, m_shape.getRadius() / 2);
}

void Ball::draw(sf::RenderWindow& window)
{
	window.draw(m_shape);
	window.draw(m_debug);
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
void Ball::move(float dt, sf::RenderWindow& window) // this method is used to move the ball based on its current speed and velocity
{

	// adjusting the bounds checking of the ball to better avoid it getting stuck at the top of the screen by taking into accoutn the radius of the ball for both checks 
	if (m_shape.getPosition().y - m_shape.getRadius() <= 0) {

		m_shape.setPosition(m_shape.getPosition().x, 0 + m_shape.getRadius()); // ensure to move the ball back by half before inverting the velocity 
		m_velocity.y = -m_velocity.y; // invert the balls velocity making it go a different direction 
	}
	else if (m_shape.getPosition().y + m_shape.getRadius()  >= window.getSize().y) {

		m_shape.setPosition(m_shape.getPosition().x, window.getSize().y - m_shape.getRadius());
		m_velocity.y = -m_velocity.y; // invert the balls velocity making it go a different direction 
	}
	// both condtions above are used to change the direction of the ball each time it hits the top edge or bottm edge of the screen  in order to create a bouncing effect
	m_shape.move(m_velocity * dt); // call the move method associtated with the CircleShape class that the m_shape attribute is an object of which will move the shape by an offest of the velocity attribute(an sf::vector2f object) multiplied by delta time(an argument passed into this method) 
}



sf::Vector2f Ball::getPosition()
{
	return m_shape.getPosition(); // return the resul of calling the get postion method on the m_shape attribute contained within the ball class 
	// this wil return an sfml vector2f object representing the current coordinates of the m_shape object  
}



bool Ball::ballCollisionPushBack(sf::RectangleShape paddleBounds) {


	// this code is a rectangle to cricle intersection algorithm and has been implemnted for better accuracy of collsion 
	// rather than just using the two bounding boxes of the CircleShape and the rectangle sfml provides 
	// in order to generate the collsion particles when the ball collides with the paddle 
	sf::Vector2f distanceBetweenBall = paddleBounds.getPosition() - m_shape.getPosition(); // get the vector between the current paddle position and the ball


	float minX = paddleBounds.getPosition().x - paddleBounds.getSize().x/2; //get the minmum top left cordinate of the paddle we are checking on the x axis 
	float minY = paddleBounds.getPosition().y - paddleBounds.getSize().y/2; // get the minumum top left cordinate of the paddle we are checking on the y axis 

	
	// below will compute the surafce point collsion on the paddle that the ball is likley to hit by clamping the balls current postion within the range of the paddles bounding rectangle essentially representing the balls postion 
	// but  restricting it to the bounds of the paddle 
	
	// this means that the surfacePoint on the x axis that the ball can collide with will always be the minimum x coordinate of the reactangle or the maximum x coordinate the paddle is rapped in 
	// and the maximum surfaceY collsion point will always be the height of the paddle added to its  minY cordinate(top left y coordinate)

	// for this case only the surfacePointY will change as the paddle is restricted to vertcial movement 
	// meaning that the surfacePoint of collsion will move up and down in the Y axis with the ball but be restcited to the current toy left Y coordinate of the paddle 
	// and the bottom Y cordinate of the paddles corners essentially REPLICATING the balls current postion within the bounds of each paddle  
	/// this is achived by using the min  and max functions below 

	// get the minumum value between the balls psotion and the maximum x cordinate the paddle covers ensuring that the surfacePoint of collision stays within the bounds 
	// of the paddle on the x axis than max that with the minimumX cordinate of the paddle this measn that if the ball isnt within the paddle we will always get the postion of the surface point at minimum 
	// or maximum X depending on if the ball is on the left or right of the paddle(same for y)
	float surfacePointX = std::max(minX,std::min(m_shape.getPosition().x, minX + paddleBounds.getGlobalBounds().width));  
	float surfacePointY = std::max(minY, std::min(m_shape.getPosition().y, minY + paddleBounds.getGlobalBounds().height));

	//m_debug.setPosition(surfacePointX, surfacePointY);

	sf::Vector2f nearestPointToSurface = sf::Vector2f(surfacePointX - m_shape.getPosition().x, surfacePointY - m_shape.getPosition().y); // get the differnce between the surfacePoint of collsion on the paddle and the balls actaul postion
 
	float distanceToSurfacePoint = sqrt(powf(nearestPointToSurface.x, 2) + powf(nearestPointToSurface.y, 2)); // get the maginutude of the vector between the balls postion represented in the bounds of the paddle and the balls actual postion 
	
	if (distanceToSurfacePoint < m_shape.getRadius()) { // if the distance to the surface point x and y is smaller than the balls current radius meanig that we have an overlap 
		
	    std::cout << "collsion" << std::endl;
		float overlap = m_shape.getRadius() - distanceToSurfacePoint; // calculate  the amount the ball overlapped which is the differnce bewteen the balls radius and the distance between the surface point as the ball is currenlty overlapping the paddle 
		// meaning that some part of the circle is inside which can be measured using the radius and the current distance between the centre point of the circle and the surface collision point 
		  
		std::cout << overlap << std::endl;
		sf::Vector2f normaliseCollsionVector = sf::Vector2f(nearestPointToSurface.x /= distanceToSurfacePoint, nearestPointToSurface.y /= distanceToSurfacePoint);
		// get the collsion normal which is the vector between the centre of the circle and surface point normalsied 
		
		// move the ball along the direction of inverted collsion normal so that it travels backward multiplying the 
		// collsion normal by the radius+overlap to move the ball fully outside of the paddle before registering a collision as true 
		m_shape.move(-normaliseCollsionVector * (m_shape.getRadius() + overlap)); 
	
		
		if (distanceBetweenBall.x < 0) { // if the diatnce between the ball is less then 0 then the ball is coming from the right as the paddles postion will be smaller than the ball 
			updateVelocity(1); // so we update the velcoity to be psotive making the ball move to the right again 
		}
		else {
			updateVelocity(-1); // opposite of above 
		}
		return true; // collsion happend 
	 
	}

    return false; // collsion didn't happen


	// i found that using the intersects method would often result in less precise collsion and often the code below would return true multiple times especially whe hitting the edges 
	// i found that this was due to the intersects method using the bounding box of the ball to check for the intersection therefore with this being a bounding box encompassing the ball 
	// it would often result in miss fires where there was actually no collsion at all( as the bounding box is merly a rectangle that encompasses the vertcies of the CircleShape to the best of its capabilties) 
	// resulting in inaccurate collsion with the edges of the paddle(and a lot more collsion particles than intended)
	/*if (paddleBounds.getGlobalBounds().intersects(m_shape.getGlobalBounds())) {
       
		if (distanceBetweenBall.x < 0) {
			updateVelocity(1);
		}
		else {
			updateVelocity(-1);
		}
		return true;
        



	}
	return false;*/
	
	

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




