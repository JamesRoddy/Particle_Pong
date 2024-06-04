#include "Menu.h"


menu::menu(float fWidth, float fHeight, sf::Vector2f fButtonSize, unsigned int iTextCharSize) {

	m_windowHeight = fHeight;
	m_windowWidth = fWidth;
	m_buttonSize = fButtonSize;
	m_textCharacterSize = iTextCharSize;
	m_textFont.loadFromFile(".\\assets\\fonts\\impact.ttf");
	buttonHighLightColour = sf::Color::White;
	textColour = sf::Color::White;
	buttonColour = sf::Color(128, 128, 128, 255);

	setMenu();
}

void menu::setMenu() {

	for (int i = 0; i < iMenuButtonNumber; i++) {

		m_buttons[i].setFillColor(buttonColour);
		m_buttons[i].setOrigin(m_buttonSize / 2.f);
		m_buttons[i].setSize(m_buttonSize);
		if (i != 0) {
			m_buttons[i].setPosition(
				m_windowWidth / 2 - m_buttonSize.x / 2,
				m_buttons[i - 1].getPosition().y + m_buttonSize.y * 2
			);

		}
		else {
			m_buttons[i].setPosition(m_windowWidth / 2 - m_buttonSize.x / 2,
				m_windowHeight / 2 - m_buttonSize.y * 2);
		}
		m_textObjects[i].setFillColor(textColour);
		m_textObjects[i].setString(m_menuText[i]);
		m_textObjects[i].setFont(m_textFont);
		m_textObjects[i].setPosition(m_buttons[i].getPosition());




	}



}

void menu::draw(sf::RenderWindow&window, bool bShouldDraw) {
	if (bShouldDraw) {
		for (int i = 0; i < iMenuButtonNumber; i++) {
			window.draw(m_buttons[i]);
			window.draw(m_textObjects[i]);

		}
	}
	

}

void menu::Update() {

	for (int iButtonIndex = 0; iButtonIndex< iMenuButtonNumber; iButtonIndex++) {
		
		if (m_buttons[iButtonIndex].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition()))) {
			m_buttons[iButtonIndex].setOutlineColor(buttonHighLightColour);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				validateOption(iButtonIndex);
			}
		}



	}
}

void menu::validateOption(int optionId) {
  
	switch (optionId) {
	case PLAY:
		m_shouldPlay = true;
    case QUIT:
		m_shouldQuit = true;

	}


}

bool menu::shouldPlay() {
	return m_shouldPlay;
}
bool menu::shouldQuit() {
	return m_shouldQuit;
}

