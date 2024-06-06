#include "Menu.h"
#include <iostream>

// Menu constrctor/initialiser that takes in the window widht and height along with button/text size for Menu objects
Menu::Menu(float fWidth, float fHeight, sf::Vector2f fButtonSize, unsigned int iTextCharSize,sf::Color textColour,float fButtonSpacing) { 

	m_windowHeight = fHeight; // setting width/height attributes
	m_windowWidth = fWidth; 
	m_buttonSize = fButtonSize; 
	m_textCharacterSize = iTextCharSize;
	m_spacing = fButtonSpacing; // set spacing for buttons
	m_textFont.loadFromFile(".\\assets\\fonts\\impact.ttf"); // load the font that will be used for the menu text objects
	m_buttonHighLightColour = sf::Color::White; // setting highlight colour
	m_outlineThickness = 10.0f;
	m_textColour = textColour;
	m_buttonColour = sf::Color(128, 128, 128, 255); // background colour for buttons
	m_headerCharacterSize = 50;

	m_headerFont.loadFromFile(".\\assets\\fonts\\digital-7.ttf");
	m_navigationBuffer.loadFromFile(".\\assets\\audio\\menuNavigateSound.wav");
	m_navigationSound.setBuffer(m_navigationBuffer);
	m_navSoundActive = false;
	setHeaderText();
	setMenuObjects(); // intialise menu and all button/text object postions and properties
}






void Menu::setHeaderText() {

	m_header.setFont(m_headerFont);
	m_header.setFillColor(m_textColour);
	m_header.setCharacterSize(m_headerCharacterSize);
    
	m_header.setPosition(static_cast<float> (m_windowWidth / 2 - m_headerCharacterSize ),
		static_cast<float>(0 + m_headerCharacterSize));
	m_header.setString(m_headerText);


}




void Menu::setMenuObjects() {

	for (int i = 0; i < m_menuButtonNumber; i++) { // for the number of  buttons in the Menu
		// set the properties of the sf rectangle shape asscoiated with each button
		m_buttons[i].setFillColor(m_buttonColour); 
		m_buttons[i].setOrigin(m_buttonSize / 2.f);
		m_buttons[i].setSize(m_buttonSize);
		// if we are not at the beginning of the array that stores the rectangle objects for buttons
		if (i != 0) {
			// set the postion of the first button to be the centre of the screen and centre y+ the previous button size multiplied by spacing
			m_buttons[i].setPosition(
				m_windowWidth / 2 ,
				m_buttons[i - 1].getPosition().y + m_buttonSize.y * m_spacing);

		}
		else {
			// intialise the first button postion in the buttons array 
			//  to be the centre of the screen with it being raised by its height based on spacing in y
			m_buttons[i].setPosition(m_windowWidth / 2 ,
				m_windowHeight / 2 - (m_buttonSize.y * m_spacing)+m_headerCharacterSize);
		}
		// intialise text object properties 
		m_textObjects[i].setFillColor(m_textColour);
		m_textObjects[i].setString(m_menuText[i]);
		m_textObjects[i].setFont(m_textFont);
		// set postion based on button postion and character size
		m_textObjects[i].setPosition(
			static_cast<float> (m_buttons[i].getPosition().x - m_textCharacterSize),
			static_cast<float>(m_buttons[i].getPosition().y - m_textCharacterSize) ); 




	}



}

///draw method for the menu
void Menu::draw(sf::RenderWindow &window, bool bShouldDraw) const {
	if (bShouldDraw) { // a boolean is passed into this method to determine whetehr or not the menu should draw
		window.draw(m_header); // draw the title text
		for (int i = 0; i < m_menuButtonNumber; i++) { // draw each menu button and its associated text
			window.draw(m_buttons[i]);
			window.draw(m_textObjects[i]);

		}
	}
	

}

void Menu::Update(sf::Window&window) {

	bool bShouldPlaySound = false;
	for (int iButtonIndex = 0; iButtonIndex < m_menuButtonNumber; iButtonIndex++) {
		// if the current button we are checking contains the mouse coordinates relative to the window 
		if (m_buttons[iButtonIndex].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
			
			m_buttons[iButtonIndex].setOutlineColor(m_buttonHighLightColour); // set the hover highlight colour
			m_buttons[iButtonIndex].setOutlineThickness(10.0f); // set the outline thickness
			bShouldPlaySound = true;
		
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { // if we capture mouse input while user is hoverning mouse of the button selected
				validateOption(iButtonIndex); //validate the option based on the current index according to the enum "options"
			}
		}
		else {
			// otherwise reset the button
			m_buttons[iButtonIndex].setOutlineColor(m_buttonColour);
			m_buttons[iButtonIndex].setOutlineThickness(0.0f);
		}
	}

	playNavSound(bShouldPlaySound);




}
void Menu::playNavSound(bool bSoundPlaying) { // used to dtermine if the menu selection sound should play
	if (bSoundPlaying && !m_navSoundActive) { // if the sound should play and hasnt already activated 
		m_navigationSound.play(); // inititae the sound
		m_navSoundActive = true; // the sound has already played so we set the boolean flag for it being actie to true
	}
	else if (!bSoundPlaying) { // otherwise if the sound shouldnt play meaning the user isnt hovering overa any buttons
		m_navSoundActive = false; // we set the boolean flag for playing the navigation sound to false 
	}
}

void Menu::validateOption(int optionId) { // used to determine how the menu reacts once the user clicks a button
  
	switch (optionId) {
	case PLAY:
		m_shouldPlay = true;
    case QUIT:
		m_shouldQuit = true;

	}


}

// methods to get the various booleans associated with menu options 
bool Menu::shouldPlay() const {
	return m_shouldPlay;
}
bool Menu::shouldQuit() const {
	return m_shouldQuit;
}
void Menu::resetOptionBools() {
	m_shouldPlay = false;
	m_shouldQuit = false;
}

