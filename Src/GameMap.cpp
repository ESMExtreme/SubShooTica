#include "GameMap.h"
#include <string>
#include <iostream>

GameMap::GameMap() : backgroundTexture(), font(), tiles(), selectedIndex(0)
{
    bool loaded = backgroundTexture.loadFromFile("Assets/Media/Mapa.png");
    if (!loaded) {
        std::cout << "Błąd: Nie można załadować Assets/Media/Mapa.png" << std::endl;
    } else {
        std::cout << "Załadowano  - rozmiar:Mapa.png " << backgroundTexture.getSize().x << "x" << backgroundTexture.getSize().y << std::endl;

        // Utwórz sprite DOPIERO po załadowaniu tekstury
        background.emplace(backgroundTexture);
        std::cout << "Sprite utworzony z teksturą!" << std::endl;
    }

    if (!font.openFromFile("Assets/Fonts/BerlinSans.ttf")) {
        std::cout << "Błąd: Nie można załadować czcionki" << std::endl;
    }

    // Tworzenie 13 tiles z liczbami 0-12 w siatce
    std::vector<sf::Vector2f> positions = {
        {755.f, 527.f},   // 0
        {827.f, 416.f},   // 1
        {931.f, 558.f},   // 2
        {612.f, 373.f},   // 3
        {569.f, 476.f},   // 4
        {442.f, 526.f},  // 5
        {815.f, 278.f},  // 6
        {995.f, 288.f},   // 7
        {1126.f, 469.f},   // 8
        {831.f, 756.f},   // 9
        {1117.f, 740.f},   // 10
        {576.f, 662.f},  // 11
        {1156.f, 844.f}   // 12
    };

    for (int i = 0; i < 13; ++i) {
        CreateTile(std::to_string(i), sf::Color::Cyan, positions[i], i, 60);
    }
}

void GameMap::CreateTile(const std::string& string, sf::Color color, sf::Vector2f position, int index, int fontSize) {
    sf::Text text(font);
    text.setString(string);
    text.setCharacterSize(fontSize);
    text.setFillColor(color);
    text.setPosition(position);
    tiles.push_back(text);
}

void GameMap::handleEvent(const sf::Event& event, int* currentScreen) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        using Key = sf::Keyboard::Key;
        if (key->code == Key::Left) {
            selectedIndex = (selectedIndex - 1 + 13) % 13;
        } else if (key->code == Key::Right) {
            selectedIndex = (selectedIndex + 1) % 13;
        } else if (key->code == Key::Up) {
            // Jeśli jesteś w rzędzie 1-6 (indeksy 1-6), idź do 0
            // Jeśli jesteś w rzędzie 7-12 (indeksy 7-12), idź do odpowiedniego w górnym rzędzie (7->1, 8->2, itd.)
            if (selectedIndex >= 7 && selectedIndex <= 12) {
                selectedIndex = selectedIndex - 6;
            } else if (selectedIndex >= 1 && selectedIndex <= 6) {
                selectedIndex = 0;
            }
        } else if (key->code == Key::Down) {
            // Z 0 idź do 1
            // Z rzędu 1-6 idź do 7-12
            if (selectedIndex == 0) {
                selectedIndex = 1;
            } else if (selectedIndex >= 1 && selectedIndex <= 6) {
                selectedIndex = selectedIndex + 6;
            } else if (selectedIndex >= 7 && selectedIndex <= 12) {
                selectedIndex = 0;
            }
        } else if (key->code == Key::Enter) {
            // Wybór poziomu w zależności od selectedIndex
            switch (selectedIndex) {
                case 0:
                    *currentScreen = 4; // Poziom 0 - GameScreen
                    break;
                case 1:
                    *currentScreen = 4; // Poziom 1 - GameScreen
                    break;
                case 2:
                    *currentScreen = 4; // Poziom 2 - GameScreen
                    break;
                case 3:
                    *currentScreen = 4; // Poziom 3 - GameScreen
                    break;
                case 4:
                    *currentScreen = 4; // Poziom 4 - GameScreen
                    break;
                case 5:
                    *currentScreen = 4; // Poziom 5 - GameScreen
                    break;
                case 6:
                    *currentScreen = 4; // Poziom 6 - GameScreen
                    break;
                case 7:
                    *currentScreen = 4; // Poziom 7 - GameScreen
                    break;
                case 8:
                    *currentScreen = 4; // Poziom 8 - GameScreen
                    break;
                case 9:
                    *currentScreen = 4; // Poziom 9 - GameScreen
                    break;
                case 10:
                    *currentScreen = 4; // Poziom 10 - GameScreen
                    break;
                case 11:
                    *currentScreen = 4; // Poziom 11 - GameScreen
                    break;
                case 12:
                    *currentScreen = 4; // Poziom 12 - GameScreen
                    break;
                default:
                    *currentScreen = 4; // Domyślnie GameScreen
                    break;
            }
        } else if (key->code == Key::Escape) {
            *currentScreen = 0; // Powrót do menu
        }
    }
}

void GameMap::draw(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();

    // Rysuj tło
    if (background.has_value() && textureSize.x > 0 && textureSize.y > 0) {
        float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
        float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);
        background->setScale({scaleX, scaleY});
        window.draw(*background);
    }

    // Rysuj tiles (liczby) z podświetleniem wybranego
    for (size_t i = 0; i < tiles.size(); ++i) {
        // Cyan dla niewybranych, Dark Blue (0, 0, 139) dla wybranego
        tiles[i].setFillColor(static_cast<int>(i) == selectedIndex ? sf::Color(0, 0, 139) : sf::Color::Cyan);
        window.draw(tiles[i]);
    }
}