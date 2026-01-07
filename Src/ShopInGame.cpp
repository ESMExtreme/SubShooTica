#include "ShopInGame.h"
#include <iostream>
#include <fstream>
#include <string>

ShopInGame::ShopInGame() : selectedIndex(0), currentSaveSlot(0), playerScrap(0),
                           titleText(), scrapText() {
    // Załaduj czcionkę
    if (!font.openFromFile("Assets/Fonts/BerlinSans.ttf")) {
        std::cerr << "Błąd: Nie można załadować czcionki dla sklepu" << std::endl;
    }

    // Tytuł sklepu
    titleText.emplace(font);
    titleText->setString("SKLEP");
    titleText->setCharacterSize(60);
    titleText->setFillColor(sf::Color::Yellow);
    titleText->setPosition(sf::Vector2f(800.f, 50.f));

    // Tekst ze złomem
    scrapText.emplace(font);
    scrapText->setCharacterSize(40);
    scrapText->setFillColor(sf::Color::Green);
    scrapText->setPosition(sf::Vector2f(50.f, 50.f));


    createShopItems();
}

void ShopInGame::createShopItems() {
    items.clear();
    itemTexts.clear();
    itemBoxes.clear();

    // Definicja przedmiotów w sklepie
    items.push_back({"Szybsze Pociski", "+50% predkosc pociskow", 100, false});
    items.push_back({"Wieksze Pociski", "+100% rozmiar pociskow", 150, false});
    items.push_back({"Szybszy Ruch", "+50% predkosc ruchu", 200, false});
    items.push_back({"Podwojne Zycie", "+100% HP", 300, false});
    items.push_back({"Szybsze Strzelanie", "-50% cooldown", 250, false});

    // Tworzenie wizualnych elementów
    float yPos = 200.f;
    for (size_t i = 0; i < items.size(); ++i) {
        // Prostokąt dla przedmiotu
        sf::RectangleShape box;
        box.setSize(sf::Vector2f(1200.f, 100.f));
        box.setPosition(sf::Vector2f(360.f, yPos));
        box.setFillColor(sf::Color(50, 50, 50));
        box.setOutlineThickness(3.f);
        box.setOutlineColor(sf::Color::White);
        itemBoxes.push_back(box);

        // Tekst przedmiotu
        sf::Text text(font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(380.f, yPos + 10.f));
        itemTexts.push_back(text);

        yPos += 130.f;
    }

    updateDisplay();
}

void ShopInGame::updateDisplay() {
    if (scrapText.has_value()) {
        scrapText->setString("Zlom: " + std::to_string(playerScrap));
    }

    for (size_t i = 0; i < items.size(); ++i) {
        std::string displayText = items[i].name + " - $" + std::to_string(items[i].price);
        if (items[i].purchased) {
            displayText += " [KUPIONE]";
            itemTexts[i].setFillColor(sf::Color(100, 100, 100));
        } else {
            displayText += "\n" + items[i].description;
            itemTexts[i].setFillColor(sf::Color::White);
        }
        itemTexts[i].setString(displayText);

        // Podświetl wybrany przedmiot
        if (static_cast<int>(i) == selectedIndex) {
            itemBoxes[i].setOutlineColor(sf::Color::Yellow);
            itemBoxes[i].setOutlineThickness(5.f);
        } else {
            itemBoxes[i].setOutlineColor(sf::Color::White);
            itemBoxes[i].setOutlineThickness(3.f);
        }
    }
}

void ShopInGame::handleEvent(const sf::Event& event, int* currentScreen) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        using Key = sf::Keyboard::Key;

        if (key->code == Key::Up) {
            selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
            updateDisplay();
        } else if (key->code == Key::Down) {
            selectedIndex = (selectedIndex + 1) % items.size();
            updateDisplay();
        } else if (key->code == Key::Enter) {
            // Enter tylko kupuje przedmioty
            purchaseItem();
        } else if (key->code == Key::Escape) {
            saveShopData(); // Zapisz stan sklepu
            *currentScreen = *currentScreen - 1; // Wróć do poprzedniego ekranu
        }
    }
}

void ShopInGame::purchaseItem() {
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size())) {
        ShopItem& item = items[selectedIndex];

        if (!item.purchased && playerScrap >= item.price) {
            item.purchased = true;
            playerScrap -= item.price;
            updateDisplay();
            std::cout << "Kupiono: " << item.name << std::endl;
        } else if (item.purchased) {
            std::cout << "Przedmiot już kupiony!" << std::endl;
        } else {
            std::cout << "Za mało pieniędzy!" << std::endl;
        }
    }
}

void ShopInGame::update(float deltaTime, sf::Vector2u windowSize) {
    // Aktualizacja (jeśli potrzebna)
}

void ShopInGame::draw(sf::RenderWindow& window) {
    // Rysuj tło
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(static_cast<float>(window.getSize().x),
                                    static_cast<float>(window.getSize().y)));
    background.setFillColor(sf::Color(20, 20, 30));
    window.draw(background);

    // Rysuj tytuł i złom
    if (titleText.has_value()) {
        window.draw(*titleText);
    }
    if (scrapText.has_value()) {
        window.draw(*scrapText);
    }

    // Rysuj przedmioty
    for (size_t i = 0; i < itemBoxes.size(); ++i) {
        window.draw(itemBoxes[i]);
        window.draw(itemTexts[i]);
    }
}

std::string ShopInGame::getSaveFileName() const {
    return "Assets/shop" + std::to_string(currentSaveSlot) + ".dat";
}

void ShopInGame::setSaveSlot(int slot) {
    currentSaveSlot = slot;
    loadShopData();
}

void ShopInGame::loadShopData() {
    std::string filename = getSaveFileName();
    std::ifstream file(filename);

    if (file.is_open()) {
        // NIE wczytuj pieniędzy - są tylko w save*.dat!
        // Wczytaj wszystkie przedmioty (5 przedmiotów, bez START GAME)
        for (size_t i = 0; i < items.size(); ++i) {
            int purchased;
            file >> purchased;
            items[i].purchased = (purchased == 1);
        }
        file.close();
        std::cout << "Wczytano dane sklepu z: " << filename << std::endl;
    } else {
        // Nowy sklep - domyślne wartości
        // playerScrap pozostaje jak jest (z save*.dat przez setPlayerScrap)
        for (auto& item : items) {
            item.purchased = false;
        }
        std::cout << "Nowy sklep dla slotu " << currentSaveSlot << std::endl;
    }

    updateDisplay();
}

void ShopInGame::saveShopData() {
    std::string filename = getSaveFileName();
    std::ofstream file(filename);

    if (file.is_open()) {
        // NIE zapisuj pieniędzy - są tylko w save*.dat!
        // Zapisz wszystkie przedmioty (5 przedmiotów, bez START GAME)
        for (size_t i = 0; i < items.size(); ++i) {
            file << (items[i].purchased ? 1 : 0) << "\n";
        }
        file.close();
        std::cout << "Zapisano dane sklepu do: " << filename << std::endl;
    } else {
        std::cerr << "Błąd: Nie można zapisać danych sklepu do: " << filename << std::endl;
    }
}

