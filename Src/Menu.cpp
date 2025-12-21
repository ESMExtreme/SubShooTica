#include "Menu.h"
#include <iostream>

Menu::Menu(sf::RenderWindow& window) {
    if (!font.loadFromFile("./Media/arial.ttf")) {
        std::cerr << "Error loading font\n";
    }

    title01.setFont(font);
    title01.setString("SubShooTica");
    title01.setCharacterSize(50);
    title01.setFillColor(sf::Color::White);
    title01.setOrigin(title01.getLocalBounds().width / 2, title01.getLocalBounds().height / 2);
    title01.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 50);

    title02.setFont(font);
    title02.setString("Wcisnij ENTER aby zagrac");
    title02.setCharacterSize(20);
    title02.setFillColor(sf::Color::White);
    title02.setOrigin(title02.getLocalBounds().width / 2, title02.getLocalBounds().height / 2);
    title02.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 20);
}

bool Menu::run(sf::RenderWindow& window) {
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    return true;
                }
            }
        }

        window.clear();
        window.draw(title01);
        window.draw(title02);
        window.display();
    }
    return false;
}
