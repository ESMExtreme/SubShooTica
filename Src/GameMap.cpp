#include "GameMap.h"
#include <string>
#include <iostream>

GameMap::GameMap() : currentBackgroundTexture(), font(), tiles(), selectedIndex(0), currentSaveSlot(0), saveData()
{
    if (!font.openFromFile("Assets/Fonts/BerlinSans.ttf")) {
        std::cout << "Błąd: Nie można załadować czcionki" << std::endl;
    }

    // Tło zostanie załadowane później w setSaveSlot() po wczytaniu SaveData

    // Tworzenie 13 tiles z liczbami 0-12 w siatce
    std::vector<sf::Vector2f> positions = {
        {350.f, 50.f},   // 0
        {150.f, 200.f},  // 1
        {300.f, 200.f},  // 2
        {450.f, 200.f},  // 3
        {600.f, 200.f},  // 4
        {750.f, 200.f},  // 5
        {900.f, 200.f},  // 6
        {150.f, 400.f},  // 7
        {300.f, 400.f},  // 8
        {450.f, 400.f},  // 9
        {600.f, 400.f},  // 10
        {750.f, 400.f},  // 11
        {900.f, 400.f}   // 12
    };

}

void GameMap::setSaveSlot(int slot) {
    currentSaveSlot = slot;
    // Wczytaj dane zapisu
    std::string saveFile = "Assets/save" + std::to_string(currentSaveSlot) + ".json";
    if (SaveData::fileExists(saveFile)) {
        saveData.loadFromFile(saveFile);
    }
    // Załaduj tło dla obecnie wybranego levelu
    loadBackground(selectedIndex);
}

void GameMap::loadBackground(int levelIndex) {
    std::string backgroundPath;

    // Sprawdź czy level jest odblokowany
    bool isUnlocked = (levelIndex <= saveData.maxLevelUnlocked);

    // Wybierz odpowiednie tło: U*.png (unlocked) lub L*.png (locked)
    if (isUnlocked) {
        backgroundPath = "Assets/Media/Map/unlocked/U" + std::to_string(levelIndex) + ".png";
    } else {
        backgroundPath = "Assets/Media/Map/locked/L" + std::to_string(levelIndex) + ".png";
    }

    std::cout << "Próba załadowania: " << backgroundPath << std::endl;
    std::cout << "Level " << levelIndex << " unlocked=" << isUnlocked << " maxUnlocked=" << saveData.maxLevelUnlocked << std::endl;

    // WAŻNE: Najpierw usuń stary sprite i teksturę
    background.reset();

    // Utwórz NOWĄ teksturę (wyczyść starą)
    sf::Texture newTexture;
    bool loaded = newTexture.loadFromFile(backgroundPath);

    if (!loaded) {
        std::cout << "BŁĄD: Nie można załadować " << backgroundPath << std::endl;
        return;
    }

    std::cout << "✓ Załadowano tło: " << backgroundPath << " (rozmiar: " << newTexture.getSize().x << "x" << newTexture.getSize().y << ")" << std::endl;

    // Przypisz nową teksturę
    currentBackgroundTexture = std::move(newTexture);

    // Utwórz NOWY sprite z nową teksturą
    background.emplace(currentBackgroundTexture);
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
            loadBackground(selectedIndex); // Załaduj nowe tło dla wybranego levelu
        } else if (key->code == Key::Right) {
            selectedIndex = (selectedIndex + 1) % 13;
            loadBackground(selectedIndex); // Załaduj nowe tło dla wybranego levelu
        } else if (key->code == Key::Up) {
            if (selectedIndex >= 7 && selectedIndex <= 12) {
                selectedIndex = selectedIndex - 6;
            } else if (selectedIndex >= 1 && selectedIndex <= 6) {
                selectedIndex = 0;
            }
            loadBackground(selectedIndex); // Załaduj nowe tło dla wybranego levelu
        } else if (key->code == Key::Down) {
            if (selectedIndex == 0) {
                selectedIndex = 1;
            } else if (selectedIndex >= 1 && selectedIndex <= 6) {
                selectedIndex = selectedIndex + 6;
            } else if (selectedIndex >= 7 && selectedIndex <= 12) {
                selectedIndex = 0;
            }
            loadBackground(selectedIndex); // Załaduj nowe tło dla wybranego levelu
        } else if (key->code == Key::Enter) {
            // Sprawdź czy level jest odblokowany przed wejściem
            bool isUnlocked = (selectedIndex <= saveData.maxLevelUnlocked);
            if (!isUnlocked) {
                std::cout << "Level " << selectedIndex << " jest zablokowany!" << std::endl;
                return; // Nie pozwól wejść do zablokowanego levelu
            }

            // Wybór poziomu w zależności od selectedIndex
            if (selectedIndex == 0) {
                *currentScreen = 4; // Poziom 0 - Sklep
            } else {
                *currentScreen = 5; // Poziomy 1-12 - bezpośrednio do gry
            }
        } else if (key->code == Key::Escape) {
            *currentScreen = *currentScreen - 1; // Cofnij do poprzedniego ekranu (SaveScreen)
        }
    }
}

void GameMap::draw(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = currentBackgroundTexture.getSize();

    // Rysuj tło
    if (background.has_value() && textureSize.x > 0 && textureSize.y > 0) {
        float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
        float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);
        background->setScale({scaleX, scaleY});
        window.draw(*background);
    }

    // Rysuj tiles (liczby) - bez zmiany koloru, tylko tło się zmienia
    for (size_t i = 0; i < tiles.size(); ++i) {
        window.draw(tiles[i]);
    }
}

