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

    // Załaduj tekstury ikonek
    if (loreIconTexture.loadFromFile("Assets/Media/ikonka_lore.png")) {
        loreIcon.emplace(loreIconTexture);
        loreIcon->setPosition(sf::Vector2f(250.f, 140.f));  // Pozycja w lewym górnym rogu
        loreIcon->setScale(sf::Vector2f(0.5f, 0.5f));     // Skalowanie (dostosuj jeśli potrzeba)

        // Tekst "B" obok ikonki lore
        loreText.emplace(font);
        loreText->setString("B");
        loreText->setCharacterSize(40);
        loreText->setFillColor(sf::Color::White);
        loreText->setPosition(sf::Vector2f(320.f, 140.f)); // Obok ikonki
    }

    if (mapIconTexture.loadFromFile("Assets/Media/ikonka_mapa.png")) {
        mapIcon.emplace(mapIconTexture);
        mapIcon->setPosition(sf::Vector2f(250.f, 200.f));  // Pod ikonką lore
        mapIcon->setScale(sf::Vector2f(0.5f, 0.5f));

        // Tekst "N" obok ikonki mapa
        mapText.emplace(font);
        mapText->setString("M");
        mapText->setCharacterSize(40);
        mapText->setFillColor(sf::Color::White);
        mapText->setPosition(sf::Vector2f(320.f, 200.f));
    }

    if (shopIconTexture.loadFromFile("Assets/Media/ikonka_sklep.png")) {
        shopIcon.emplace(shopIconTexture);
        shopIcon->setPosition(sf::Vector2f(250.f, 260.f));  // Pod ikonką mapa
        shopIcon->setScale(sf::Vector2f(0.5f, 0.5f));

        // Tekst "M" obok ikonki sklep
        shopText.emplace(font);
        shopText->setString("N");
        shopText->setCharacterSize(40);
        shopText->setFillColor(sf::Color::White);
        shopText->setPosition(sf::Vector2f(320.f, 260.f));
    }
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
        } else if (key->code == Key::B) {
            // Przejdź do ekranu Lore (currentScreen 6)
            *currentScreen = 6;
        } else if (key->code == Key::N) {
            // Przejdź do sklepu (currentScreen 4)
            *currentScreen = 4;
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

    // Rysuj ikonki
    if (loreIcon.has_value()) {
        window.draw(*loreIcon);
    }
    if (mapIcon.has_value()) {
        window.draw(*mapIcon);
    }
    if (shopIcon.has_value()) {
        window.draw(*shopIcon);
    }

    // Rysuj teksty obok ikonek
    if (loreText.has_value()) {
        window.draw(*loreText);
    }
    if (mapText.has_value()) {
        window.draw(*mapText);
    }
    if (shopText.has_value()) {
        window.draw(*shopText);
    }
}

