#pragma once

#include "SFML/Graphics.hpp"

class menu {

public:
	menu(float fWindowWidth, float fWindowHeight,sf::Vector2f fButtonSize,unsigned int iTextCharSize );
	void setMenu();
	void draw(sf::RenderWindow& window,bool bShouldDraw);
	void validateOption(int iOptionId);
	void Update();
	bool shouldQuit();
	bool shouldPlay();
private:
	
	static const int iMenuButtonNumber = 2;
	sf::RectangleShape m_buttons[iMenuButtonNumber] = { sf::RectangleShape(),sf::RectangleShape() };
	sf::Text m_textObjects[iMenuButtonNumber] = { sf::Text(),sf::Text()};
	std::string m_menuText[iMenuButtonNumber] = { "PLAY","QUIT" };
	enum options{PLAY,QUIT};


	sf::Font m_textFont;
	sf::Color buttonColour = sf::Color(128, 128, 128, 255);
	sf::Color textColour = sf::Color::White;
	sf::Color buttonHighLightColour = sf::Color::White;
	float m_windowWidth;
	float m_windowHeight;
	sf::Vector2f m_buttonSize;
    bool m_shouldQuit = false;
	bool m_shouldPlay = false;
	unsigned int m_textCharacterSize;

    
	



};

