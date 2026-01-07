#include "SaveScreen.h"
#include <SFML/Graphics.hpp>

SaveScreen::SaveScreen() {
    // Ustawienie fontu
    if (!font.openFromFile("Assets/Fonts/BerlinSans.ttf")) {
        // handle error (log, throw, fallback). For now we leave an inline comment.
    }

    // Utworzenie przycisków zapisu
    CreateTile("Zapis 1", sf::Color::Red, { 400, 200 }, 0, 70);
    CreateTile("Zapis 2", sf::Color::White, { 400, 350 }, 1, 70);
    CreateTile("Zapis 3", sf::Color::White, { 400, 500 }, 2, 70);
    CreateTile("Back", sf::Color::White, { 400, 650 }, 3, 70);

    selectedItemIndex = 0; // start with first item selected
}

SaveScreen::~SaveScreen() {
}

void SaveScreen::CreateTile(const std::string& string, sf::Color color, sf::Vector2f position, int index, int fontSize) {
    sf::Text text(font);
    text.setString(string);
    text.setCharacterSize(fontSize);
    text.setFillColor(color);
    text.setPosition(position);
    saveMenu.push_back(text);
}


void SaveScreen::MoveUp() {
    saveMenu[selectedItemIndex].setFillColor(sf::Color::White);
    if (selectedItemIndex == 0)
        selectedItemIndex = static_cast<int>(saveMenu.size()) - 1;
    else
        --selectedItemIndex;
    saveMenu[selectedItemIndex].setFillColor(sf::Color::Red);
}

void SaveScreen::MoveDown() {
    saveMenu[selectedItemIndex].setFillColor(sf::Color::White);
    if (selectedItemIndex == static_cast<int>(saveMenu.size()) - 1)
        selectedItemIndex = 0;
    else
        ++selectedItemIndex;
    saveMenu[selectedItemIndex].setFillColor(sf::Color::Red);
}

void SaveScreen::handleEvent(const sf::Event& event, int* currentScreen) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        using Key = sf::Keyboard::Key;
        if (key->code == Key::Up) {
            MoveUp();
            return;
        }
        if (key->code == Key::Down) {
            MoveDown();
            return;
        }
        if (key->code == Key::Enter) {
            switch (selectedItemIndex) {
            case 0:
                *currentScreen = 3;
                break;
            case 1:
                *currentScreen = 3;
                break;
            case 2:
                *currentScreen = 3;
                break;
            case 3:
                // Back selected
                *currentScreen = 0; // Switch to main menu
                break;
            }
        }
    }
}

void SaveScreen::keyPressHandler(const sf::Event::KeyPressed* key, int* currentScreen) {
    using Key = sf::Keyboard::Key;
    if (key->code == Key::Up) {
        MoveUp();
        return;
    }
    if (key->code == Key::Down) {
        MoveDown();
        return;
    }
    if (key->code == Key::Enter) {
        switch (selectedItemIndex) {
        case 0:
            *currentScreen = 3;
            break;
        case 1:
            *currentScreen = 3;
            break;
        case 2:
            *currentScreen = 3;
            break;
        case 3:
            // Back selected
            *currentScreen = 0; // Switch to main menu
            break;
        }
    }
}

void SaveScreen::draw(sf::RenderWindow& window) {
    for (auto& text : saveMenu) {
        window.draw(text);
    }
}
