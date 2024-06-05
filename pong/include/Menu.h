#pragma once

#include "SFML/Graphics.hpp"

class menu {

public:
	menu(float fWindowWidth, float fWindowHeight,sf::Vector2f fButtonSize,unsigned int iTextCharSize );
	
	void draw(sf::RenderWindow& window,bool bShouldDraw);
	void validateOption(int iOptionId);
	void Update(sf::Window &window);
	bool shouldQuit();
	bool shouldPlay();
	void resetOptionBools();
private:
	void setMenuObjects();
	void setHeaderText();
	static const int m_menuButtonNumber = 2;
	sf::RectangleShape m_buttons[m_menuButtonNumber] = { sf::RectangleShape(),sf::RectangleShape() }; // in built sfml rectangle shape objects used to represent buttons
	sf::Text m_textObjects[m_menuButtonNumber] = { sf::Text(),sf::Text()}; // text objects that will be postioned and styled accodring to attributes of the menu class and the postion of the rectangle shape objects
	std::string m_menuText[m_menuButtonNumber] = { "PLAY","QUIT" }; // text to be assgined to the text objects
	std::string m_headerText = "PONG";

	enum options{PLAY,QUIT}; // as set of enum constants to determine menu options
	sf::Text m_header;
	float m_spacing; // spacing for menu attributes

	sf::Font m_textFont; // menu text font 
	sf::Font m_headerFont;
	sf::Color m_buttonColour = sf::Color(128, 128, 128, 255);// colour used for menu buttons
	sf::Color m_textColour = sf::Color::White; // text colour 
	sf::Color m_buttonHighLightColour; // used to change button hihglight on mouse hover 

	float m_outlineThickness;
	float m_windowWidth;
	float m_windowHeight;

	sf::Vector2f m_buttonSize;
	// boolean flags for dterminign what option the user selected
    bool m_shouldQuit = false; 
	bool m_shouldPlay = false;
	unsigned int m_textCharacterSize; // charcter size for text
	unsigned int m_headerCharacterSize;
    
	



};

