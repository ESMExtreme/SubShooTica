#include "ShopInGame.h"

ShopInGame::ShopInGame() : selectedIndex(0), currentSaveSlot(0), saveData(nullptr), titleText(), scrapText() {
    font.openFromFile("Assets/Fonts/BerlinSans.ttf");

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
    if (scrapText.has_value() && saveData) {
        scrapText->setString("Zlom: " + std::to_string(saveData->scrap));
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
            purchaseItem();
        } else if (key->code == Key::Escape) {
            // Zapisz wszystkie zmiany do pliku JSON przed wyjściem ze sklepu
            if (saveData) {
                std::string saveFile = "Assets/save" + std::to_string(currentSaveSlot) + ".json";
                bool success = saveData->saveToFile(saveFile);
                // Debug: sprawdź czy zapisywanie się powiodło
                if (!success) {
                    // Można dodać logowanie błędu
                }
            }
            *currentScreen = *currentScreen - 1; // Wróć do poprzedniego ekranu
        }
    }
}

void ShopInGame::purchaseItem() {
    if (!saveData) return;

    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size())) {
        ShopItem& item = items[selectedIndex];

        if (!item.purchased && saveData->scrap >= item.price) {
            item.purchased = true;
            saveData->scrap -= item.price;
            // Zapisz do SaveData
            if (selectedIndex < static_cast<int>(saveData->shopItemsPurchased.size())) {
                saveData->shopItemsPurchased[selectedIndex] = true;
            }
            // Natychmiast zapisz do pliku JSON po zakupie
            saveData->saveToFile("Assets/save" + std::to_string(currentSaveSlot) + ".json");
            updateDisplay();
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

void ShopInGame::loadFromSaveData(SaveData* saveDataPtr) {
    saveData = saveDataPtr;

    if (!saveData) return;

    // Wczytaj stan zakupów ze SaveData
    for (size_t i = 0; i < items.size() && i < saveData->shopItemsPurchased.size(); ++i) {
        items[i].purchased = saveData->shopItemsPurchased[i];
    }

    updateDisplay();
}


