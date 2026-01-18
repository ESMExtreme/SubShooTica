#include "SaveScreen.h"
#include <SFML/Graphics.hpp>

SaveScreen::SaveScreen() {
    // Załaduj tło background.png
    if (backgroundTexture.loadFromFile("Assets/Media/background.png")) {
        backgroundSprite.emplace(backgroundTexture);
    }

    // Załaduj teksturę zapis_box.png
    if (boxTexture.loadFromFile("Assets/Media/zapis_box.png")) {
        // Konfiguracja 4 boxów - pozycje i skale (możesz dostosować)
        boxConfigs = {
            {{300.f, 450.f}, {1.8f, 1.8f}},   // Box 0 - Zapis 1
            {{300.f, 550.f}, {1.8f, 1.8f}},   // Box 1 - Zapis 2
            {{300.f, 650.f}, {1.8f, 1.8f}},   // Box 2 - Zapis 3
            {{300.f, 750.f}, {1.8f, 1.8f}}    // Box 3 - Back
        };

        // Utwórz 4 sprite'y dla boxów
        for (size_t i = 0; i < 4; ++i) {
            sf::Sprite boxSprite(boxTexture);
            boxSprite.setPosition(boxConfigs[i].position);
            boxSprite.setScale(boxConfigs[i].scale);
            boxSprites.push_back(boxSprite);
        }
    }

    // Ustawienie fontu
    if (!font.openFromFile("Assets/Fonts/BerlinSans.ttf")) {
        // handle error (log, throw, fallback). For now we leave an inline comment.
    }

    // Utworzenie przycisków zapisu
    CreateTile("Zapis 1", sf::Color(255, 165, 0), { 420, 460 }, 0, 70);
    CreateTile("Zapis 2", sf::Color::White, { 420, 560 }, 1, 70);
    CreateTile("Zapis 3", sf::Color::White, { 420, 660 }, 2, 70);
    CreateTile("Back", sf::Color::White, { 420, 760 }, 3, 70);

    selectedItemIndex = 0; // start with first item selected
    lastSelectedSlot = 0; // domyślnie slot 0
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
    saveMenu[selectedItemIndex].setFillColor(sf::Color(255, 165, 0));
}

void SaveScreen::MoveDown() {
    saveMenu[selectedItemIndex].setFillColor(sf::Color::White);
    if (selectedItemIndex == static_cast<int>(saveMenu.size()) - 1)
        selectedItemIndex = 0;
    else
        ++selectedItemIndex;
    saveMenu[selectedItemIndex].setFillColor(sf::Color(255, 165, 0));
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
                lastSelectedSlot = 0;
                *currentScreen = 3;
                break;
            case 1:
                lastSelectedSlot = 1;
                *currentScreen = 3;
                break;
            case 2:
                lastSelectedSlot = 2;
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
            lastSelectedSlot = 0;
            *currentScreen = 3;
            break;
        case 1:
            lastSelectedSlot = 1;
            *currentScreen = 3;
            break;
        case 2:
            lastSelectedSlot = 2;
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
    // Rysuj tło background.png
    if (backgroundSprite.has_value()) {
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u textureSize = backgroundTexture.getSize();
        float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
        float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);
        backgroundSprite->setScale(sf::Vector2f(scaleX, scaleY));
        window.draw(*backgroundSprite);
    }

    // Rysuj 4 boxy zapis_box.png
    for (const auto& boxSprite : boxSprites) {
        window.draw(boxSprite);
    }

    // Rysuj tekst menu
    for (auto& text : saveMenu) {
        window.draw(text);
    }
}
