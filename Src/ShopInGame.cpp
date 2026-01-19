#include "ShopInGame.h"

ShopInGame::ShopInGame() : currentSaveSlot(0), saveData(nullptr), scrapText() {
    if (!font.openFromFile("Assets/Fonts/BerlinSans.ttf")) {
        // Handle error
    }

    // Załaduj tło bg_S&L.png
    if (backgroundTexture.loadFromFile("Assets/Media/bg_SnL.png")) {
        background.emplace(backgroundTexture);
    }

    // Tekst ze złomem
    scrapText.emplace(font, "Zlom: 0", 40);
    scrapText->setFillColor(sf::Color::Yellow);
    scrapText->setPosition(sf::Vector2f(250.f, 90.f));

    // Załaduj tekstury ikonek (tak jak na mapie)
    if (loreIconTexture.loadFromFile("Assets/Media/ikonka_lore.png")) {
        loreIcon.emplace(loreIconTexture);
        loreIcon->setPosition(sf::Vector2f(250.f, 140.f));
        loreIcon->setScale(sf::Vector2f(0.5f, 0.5f));

        // Tekst "B" obok ikonki lore
        loreText.emplace(font, "B", 40);
        loreText->setFillColor(sf::Color::White);
        loreText->setPosition(sf::Vector2f(320.f, 140.f));
    }

    if (mapIconTexture.loadFromFile("Assets/Media/ikonka_mapa.png")) {
        mapIcon.emplace(mapIconTexture);
        mapIcon->setPosition(sf::Vector2f(250.f, 200.f));
        mapIcon->setScale(sf::Vector2f(0.5f, 0.5f));

        // Tekst "M" obok ikonki mapa
        mapText.emplace(font, "M", 40);
        mapText->setFillColor(sf::Color::White);
        mapText->setPosition(sf::Vector2f(320.f, 200.f));
    }

    if (shopIconTexture.loadFromFile("Assets/Media/ikonka_sklep.png")) {
        shopIcon.emplace(shopIconTexture);
        shopIcon->setPosition(sf::Vector2f(250.f, 260.f));
        shopIcon->setScale(sf::Vector2f(0.5f, 0.5f));

        // Tekst "N" obok ikonki sklep
        shopText.emplace(font, "N", 40);
        shopText->setFillColor(sf::Color::White);
        shopText->setPosition(sf::Vector2f(320.f, 260.f));
    }

    // Inicjalizuj przedmioty sklepu
    initializeShopItems();

    // Tekst instrukcji
    instructionText.emplace(font, "Nacisnij 1-7 aby kupic przedmiot", 25);
    instructionText->setFillColor(sf::Color::White);
    instructionText->setPosition(sf::Vector2f(650.f, 850.f));
}

void ShopInGame::handleEvent(const sf::Event& event, int* currentScreen) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        using Key = sf::Keyboard::Key;

        if (key->code == Key::Escape) {
            // Zapisz wszystkie zmiany do pliku JSON przed wyjściem ze sklepu
            if (saveData) {
                std::string saveFile = "Assets/save" + std::to_string(currentSaveSlot) + ".json";
                saveData->saveToFile(saveFile);
            }
            *currentScreen = *currentScreen - 1; // Wróć do poprzedniego ekranu
        } else if (key->code == Key::B) {
            // Przejdź do ekranu Lore (currentScreen 6)
            *currentScreen = 6;
        } else if (key->code == Key::M) {
            // Przejdź do mapy (currentScreen 3)
            *currentScreen = 3;
        }
        // Kup przedmioty (klawisze 1-7)
        else if (saveData) {
            int itemIndex = -1;

            if (key->code == Key::Num1 || key->code == Key::Numpad1) itemIndex = 0;
            else if (key->code == Key::Num2 || key->code == Key::Numpad2) itemIndex = 1;
            else if (key->code == Key::Num3 || key->code == Key::Numpad3) itemIndex = 2;
            else if (key->code == Key::Num4 || key->code == Key::Numpad4) itemIndex = 3;
            else if (key->code == Key::Num5 || key->code == Key::Numpad5) itemIndex = 4;
            else if (key->code == Key::Num6 || key->code == Key::Numpad6) itemIndex = 5;
            else if (key->code == Key::Num7 || key->code == Key::Numpad7) itemIndex = 6;

            // Sprawdź czy można kupić przedmiot
            if (itemIndex >= 0 && itemIndex < static_cast<int>(shopItems.size())) {
                ShopItem& item = shopItems[itemIndex];

                // Sprawdź czy przedmiot nie jest już kupiony
                if (item.itemIndex < saveData->shopItemsPurchased.size() &&
                    !saveData->shopItemsPurchased[item.itemIndex]) {

                    // Sprawdź czy gracz ma wystarczająco złomu
                    if (saveData->scrap >= item.price) {
                        // Kup przedmiot
                        saveData->scrap -= item.price;
                        saveData->shopItemsPurchased[item.itemIndex] = true;

                        // Zastosuj efekt przedmiotu
                        applyShopBonus(item.itemIndex);

                        // Aktualizuj wyświetlanie
                        updateShopItemsDisplay();

                        // Zapisz zmiany
                        std::string saveFile = "Assets/save" + std::to_string(currentSaveSlot) + ".json";
                        saveData->saveToFile(saveFile);
                    }
                }
            }
        }
    }
}

void ShopInGame::update(float deltaTime, sf::Vector2u windowSize) {
    // Aktualizacja ilości złomu
    if (scrapText.has_value() && saveData) {
        scrapText->setString("Zlom: " + std::to_string(saveData->scrap));
    }
}

void ShopInGame::draw(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();

    // Rysuj tło bg_SnL.png
    if (background.has_value() && backgroundTexture.getSize().x > 0 && backgroundTexture.getSize().y > 0) {
        sf::Vector2u textureSize = backgroundTexture.getSize();
        float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
        float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);
        background->setScale(sf::Vector2f(scaleX, scaleY));
        window.draw(*background);
    }

    // Rysuj tekst ze złomem
    if (scrapText.has_value()) {
        window.draw(*scrapText);
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

    // Rysuj przedmioty
    for (size_t i = 0; i < shopItems.size(); ++i) {
        const auto& item = shopItems[i];

        // Rysuj ramkę wokół przedmiotu
        sf::RectangleShape itemBorder;
        itemBorder.setSize(sf::Vector2f(250.f, 200.f));
        itemBorder.setPosition(sf::Vector2f(item.position.x - 10.f, item.position.y - 10.f));
        itemBorder.setFillColor(sf::Color::Transparent);
        itemBorder.setOutlineThickness(2.f);

        // Kolor ramki zależny od statusu
        if (saveData &&
            item.itemIndex < saveData->shopItemsPurchased.size() &&
            saveData->shopItemsPurchased[item.itemIndex]) {
            itemBorder.setOutlineColor(sf::Color::Green); // Kupiony - zielony
        } else if (saveData && saveData->scrap >= item.price) {
            itemBorder.setOutlineColor(sf::Color::Yellow); // Można kupić - żółty
        } else {
            itemBorder.setOutlineColor(sf::Color(100, 100, 100)); // Za drogi - szary
        }
        window.draw(itemBorder);

        // Rysuj numer przedmiotu (1-7)
        sf::Text numberText(font);
        numberText.setString("[" + std::to_string(i + 1) + "]");
        numberText.setCharacterSize(22);
        numberText.setFillColor(sf::Color::Cyan);
        numberText.setPosition(sf::Vector2f(item.position.x, item.position.y - 35.f));
        window.draw(numberText);

        // Rysuj sprite przedmiotu
        if (item.sprite.has_value()) {
            window.draw(*item.sprite);
        }

        // Rysuj tekst nazwy przedmiotu
        if (item.nameText.has_value()) {
            window.draw(*item.nameText);
        }

        // Rysuj tekst ceny
        if (item.priceText.has_value()) {
            window.draw(*item.priceText);
        }

        // Rysuj tekst statusu
        if (item.statusText.has_value()) {
            window.draw(*item.statusText);
        }
    }

    // Nie rysuj instructionText
    // if (instructionText.has_value()) {
    //     window.draw(*instructionText);
    // }
}

void ShopInGame::loadFromSaveData(SaveData* saveDataPtr) {
    saveData = saveDataPtr;

    // Aktualizuj wyświetlanie złomu
    if (scrapText.has_value() && saveData) {
        scrapText->setString("Zlom: " + std::to_string(saveData->scrap));
    }

    // Aktualizuj wyświetlanie statusu przedmiotów
    updateShopItemsDisplay();
}

void ShopInGame::initializeShopItems() {
    shopItems.clear();

    // Definicje przedmiotów (nazwa, opis, cena, indeks, ścieżka do tekstury)
    struct ItemData {
        std::string name;
        std::string desc;
        int price;
        int index;
        std::string texturePath;
    };

    std::vector<ItemData> itemsData = {
        {"Potrójny strzał", "Strzelaj 3 pociskami naraz", 500, 0, "Assets/Media/shop_items/itemshop_3shot.png"},
        {"Zwiększony obrażenia", "+3 obrażenia za strzał", 800, 1, "Assets/Media/shop_items/itemshop_backshoot.png"},
        {"Szybki strzał I", "-0.15s szybkości strzału", 500, 2, "Assets/Media/shop_items/itemshop_quickshoot2.png"},
        {"HP Upgrade I", "+5 maksymalnego HP", 300, 3, "Assets/Media/shop_items/itemshop_hpup1.png"},
        {"HP Upgrade II", "+5 maksymalnego HP", 600, 4, "Assets/Media/shop_items/itemshop_hpup2.png"},
        {"Szybki strzał II", "-0.15s szybkości strzału", 250, 5, "Assets/Media/shop_items/itemshop_quickshoot1.png"},
        {"Cyclop Power", "Specjalna moc Cyclopa", 100, 6, "Assets/Media/shop_items/itemshop_cyclop.png"}
    };

    // Rozmieszczenie: 3 w górnym rzędzie, 4 w dolnym
    float startX = 500.f;
    float startY = 300.f;
    float spacingX = 300.f;
    float spacingY = 250.f;

    for (size_t i = 0; i < itemsData.size(); ++i) {
        ShopItem item;
        item.name = itemsData[i].name;
        item.description = itemsData[i].desc;
        item.price = itemsData[i].price;
        item.itemIndex = itemsData[i].index;

        // Oblicz pozycję (3 w rzędzie)
        int row = i / 3;
        int col = i % 3;
        item.position = sf::Vector2f(startX + col * spacingX, startY + row * spacingY);

        // Załaduj teksturę
        item.texture = std::make_shared<sf::Texture>();
        if (item.texture->loadFromFile(itemsData[i].texturePath)) {
            item.sprite.emplace(*item.texture);
            item.sprite->setPosition(item.position);

            // Skaluj sprite do rozmiarów np. 100x100
            sf::Vector2u texSize = item.texture->getSize();
            float scale = 100.f / std::max(texSize.x, texSize.y);
            item.sprite->setScale(sf::Vector2f(scale, scale));
        }

        // Tekst nazwy przedmiotu
        item.nameText.emplace(font, item.name, 20);
        item.nameText->setFillColor(sf::Color::White);
        item.nameText->setPosition(sf::Vector2f(item.position.x, item.position.y + 110.f));

        // Tekst ceny
        item.priceText.emplace(font, std::to_string(item.price) + " zlom", 18);
        item.priceText->setFillColor(sf::Color::Yellow);
        item.priceText->setPosition(sf::Vector2f(item.position.x, item.position.y + 135.f));

        // Tekst statusu (wypełniony w updateShopItemsDisplay)
        item.statusText.emplace(font, "", 18);
        item.statusText->setPosition(sf::Vector2f(item.position.x, item.position.y + 160.f));

        shopItems.push_back(item);
    }
}

void ShopInGame::updateShopItemsDisplay() {
    if (!saveData) return;

    for (auto& item : shopItems) {
        if (item.itemIndex < saveData->shopItemsPurchased.size() &&
            saveData->shopItemsPurchased[item.itemIndex]) {
            // Przedmiot kupiony
            if (item.statusText.has_value()) {
                item.statusText->setString("KUPIONO");
                item.statusText->setFillColor(sf::Color::Green);
            }
        } else {
            // Przedmiot dostępny
            if (item.statusText.has_value()) {
                item.statusText->setString("");
            }
        }
    }
}

void ShopInGame::applyShopBonus(int itemIndex) {
    if (!saveData) return;

    switch (itemIndex) {
        case 0: // Triple shoot
            saveData->tripleShoot = true;
            break;
        case 1: // +3 damage
            saveData->damageBoost += 3;
            break;
        case 2: // -0.15s fire rate
            saveData->fireRateBonus += 0.15f;
            break;
        case 3: // +5 HP
            saveData->hpBonus += 5;
            break;
        case 4: // +5 HP
            saveData->hpBonus += 5;
            break;
        case 5: // -0.15s fire rate
            saveData->fireRateBonus += 0.15f;
            break;
        case 6: // Empty or future item
            break;
    }
}

