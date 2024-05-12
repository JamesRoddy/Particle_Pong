#include "particle.h"
#include<iostream>
particle::particle(float startX, float startY, sf::Color colour, float radius) {

	particleX = startX;
	particleY = startY;
	std::cout << particleX << std::endl;
	velocity.x = speed;
	velocity.y = speed;
	particleShape.setRadius(radius);
	particleShape.setPosition(particleX, particleY);
	particleShape.setFillColor(colour);
	particleShape.setOrigin(particleShape.getRadius() / 2, particleShape.getRadius() / 2);



}

void particle::move(float dt) {
	this->particleShape.move(velocity*dt);


}
void particle::draw(sf::RenderWindow& window) {

	window.draw(this->particleShape);
	std::cout << particleShape.getPosition().x << std::endl;

}


effectGenerator::effectGenerator(unsigned int width, unsigned int height) {
	windowWidth = width;
	windowHeight = height;

}




void effectGenerator::generateParticles(int newCount, float radius) {


	float  newColorValue = rand() % 255;
	
	for (int i = 0; i < newCount; i++) {
		float randX = (rand() % int(windowWidth - radius * 2));
		float randY = (rand() % int(windowHeight - radius * 2));
		particle newParticle =  particle(randX, randY, sf::Color::Blue, radius);
		currentParticles.push_back(newParticle);

	}




}

void effectGenerator::draw(sf::RenderWindow& window) {
	for (particle newParticle : currentParticles) {
		newParticle.draw(window);
	}
}

void effectGenerator::update(float dt) {
	
	for (int i = 0; i < currentParticles.size();i++) {
		
		currentParticles[i].move(dt);
		
	}


}
