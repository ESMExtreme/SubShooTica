#include "Lore.h"
#include <fstream>
#include <sstream>

Lore::Lore() {
    // Font główny dla treści logów (Arial - pełne wsparcie polskich znaków)
    if (!font.openFromFile("Assets/Fonts/arial.ttf")) {
        // Handle error - font not loaded
    }

    // Font dla ikonek (BerlinSans - stary styl)
    if (!iconFont.openFromFile("Assets/Fonts/BerlinSans.ttf")) {
        // Handle error - font not loaded
    }

    // Załaduj wpisy lore z pliku
    loadLoreEntries();

    // Załaduj tło (używamy tego samego co sklep)
    if (backgroundTexture.loadFromFile("Assets/Media/bg_SnL.png")) {
        background.emplace(backgroundTexture);
    }

    // Treść (będzie aktualizowana)
    contentText.emplace(font, "", 30);
    contentText->setFillColor(sf::Color::White);
    contentText->setPosition(sf::Vector2f(400.f, 200.f));

    // Początkowe wyświetlenie listy
    updateDisplay();

    // Załaduj tekstury ikonek (tak jak na mapie i w sklepie)
    if (loreIconTexture.loadFromFile("Assets/Media/ikonka_lore.png")) {
        loreIcon.emplace(loreIconTexture);
        loreIcon->setPosition(sf::Vector2f(250.f, 140.f));
        loreIcon->setScale(sf::Vector2f(0.5f, 0.5f));

        // Tekst "B" obok ikonki lore - używa BerlinSans
        loreText.emplace(iconFont, "B", 40);
        loreText->setFillColor(sf::Color::White);
        loreText->setPosition(sf::Vector2f(320.f, 140.f));
    }

    if (mapIconTexture.loadFromFile("Assets/Media/ikonka_mapa.png")) {
        mapIcon.emplace(mapIconTexture);
        mapIcon->setPosition(sf::Vector2f(250.f, 200.f));
        mapIcon->setScale(sf::Vector2f(0.5f, 0.5f));

        // Tekst "M" obok ikonki mapa - używa BerlinSans
        mapText.emplace(iconFont, "M", 40);
        mapText->setFillColor(sf::Color::White);
        mapText->setPosition(sf::Vector2f(320.f, 200.f));
    }

    if (shopIconTexture.loadFromFile("Assets/Media/ikonka_sklep.png")) {
        shopIcon.emplace(shopIconTexture);
        shopIcon->setPosition(sf::Vector2f(250.f, 260.f));
        shopIcon->setScale(sf::Vector2f(0.5f, 0.5f));

        // Tekst "N" obok ikonki sklep - używa BerlinSans
        shopText.emplace(iconFont, "N", 40);
        shopText->setFillColor(sf::Color::White);
        shopText->setPosition(sf::Vector2f(320.f, 260.f));
    }
}

void Lore::handleEvent(const sf::Event& event, int* currentScreen) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        using Key = sf::Keyboard::Key;

        if (key->code == Key::Escape) {
            if (showingLoreList) {
                // Jeśli jesteśmy na liście, wróć do poprzedniego ekranu
                *currentScreen = *currentScreen - 1;
            } else {
                // Jeśli czytamy log, wróć do listy
                showingLoreList = true;
                updateDisplay();
            }
        } else if (key->code == Key::M) {
            // Przejdź do mapy (currentScreen 3)
            *currentScreen = 3;
        } else if (key->code == Key::N) {
            // Przejdź do sklepu (currentScreen 4)
            *currentScreen = 4;
        } else if (showingLoreList) {
            // Nawigacja po liście logów
            if (key->code == Key::Up) {
                selectedLoreIndex--;
                if (selectedLoreIndex < 0) {
                    selectedLoreIndex = static_cast<int>(loreEntries.size()) - 1;
                }
                updateDisplay();
            } else if (key->code == Key::Down) {
                selectedLoreIndex++;
                if (selectedLoreIndex >= static_cast<int>(loreEntries.size())) {
                    selectedLoreIndex = 0;
                }
                updateDisplay();
            } else if (key->code == Key::Enter || key->code == Key::Space) {
                // Wyświetl wybrany log
                showingLoreList = false;
                updateDisplay();
            }
        } else {
            // Gdy czytamy log, ESC już obsłużony wyżej
        }
    }
}

void Lore::update(float deltaTime, sf::Vector2u windowSize) {
    // Aktualizacja (jeśli potrzebna w przyszłości)
}

void Lore::draw(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();

    // Rysuj tło
    if (background.has_value() && backgroundTexture.getSize().x > 0 && backgroundTexture.getSize().y > 0) {
        sf::Vector2u textureSize = backgroundTexture.getSize();
        float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
        float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);
        background->setScale(sf::Vector2f(scaleX, scaleY));
        window.draw(*background);
    }

    // Rysuj tytuł
    if (titleText.has_value()) {
        window.draw(*titleText);
    }

    // Rysuj treść
    if (contentText.has_value()) {
        window.draw(*contentText);
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

void Lore::loadLoreEntries() {
    std::ifstream file("Assets/lore_entries.txt", std::ios::binary);
    if (!file.is_open()) {
        // Jeśli plik nie istnieje, dodaj domyślny wpis
        LoreEntry defaultEntry;
        defaultEntry.id = 1;
        defaultEntry.title = "Brak wpisow";
        defaultEntry.content = "Nie znaleziono pliku z wpisami lore.\nUtwórz plik Assets/lore_entries.txt";
        loreEntries.push_back(defaultEntry);
        return;
    }

    // Wczytaj cały plik do stringa
    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Usuń BOM jeśli istnieje (UTF-8 BOM: EF BB BF)
    if (fileContent.size() >= 3 &&
        static_cast<unsigned char>(fileContent[0]) == 0xEF &&
        static_cast<unsigned char>(fileContent[1]) == 0xBB &&
        static_cast<unsigned char>(fileContent[2]) == 0xBF) {
        fileContent.erase(0, 3);
    }

    // Przetwarzaj linie
    std::istringstream stream(fileContent);
    std::string line;
    LoreEntry currentEntry;
    bool readingEntry = false;
    std::string contentBuffer;

    while (std::getline(stream, line)) {
        // Usuń \r jeśli istnieje (Windows line endings)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Pomiń puste linie i komentarze
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Sprawdź czy to początek nowego wpisu [LOG_X]
        if (line[0] == '[' && line.find("LOG_") != std::string::npos) {
            // Zapisz poprzedni wpis jeśli istnieje
            if (readingEntry) {
                currentEntry.content = contentBuffer;
                loreEntries.push_back(currentEntry);
                contentBuffer.clear();
            }

            // Wyciągnij numer logu
            size_t start = line.find("LOG_") + 4;
            size_t end = line.find("]");
            if (end != std::string::npos) {
                std::string numStr = line.substr(start, end - start);
                currentEntry.id = std::stoi(numStr);
                readingEntry = true;

                // Następna linia to tytuł
                if (std::getline(stream, line)) {
                    if (!line.empty() && line.back() == '\r') {
                        line.pop_back();
                    }
                    currentEntry.title = line;
                }
            }
        } else if (readingEntry) {
            // Dodaj linię do treści
            if (!contentBuffer.empty()) {
                contentBuffer += "\n";
            }
            contentBuffer += line;
        }
    }

    // Dodaj ostatni wpis
    if (readingEntry) {
        currentEntry.content = contentBuffer;
        loreEntries.push_back(currentEntry);
    }

    // Jeśli nie załadowano żadnych wpisów
    if (loreEntries.empty()) {
        LoreEntry defaultEntry;
        defaultEntry.id = 1;
        defaultEntry.title = "Brak wpisow";
        defaultEntry.content = "Plik lore_entries.txt jest pusty.";
        loreEntries.push_back(defaultEntry);
    }
}

void Lore::updateDisplay() {
    if (loreEntries.empty()) {
        return;
    }

    if (showingLoreList) {
        // Wyświetl listę logów
        std::string header = "Wybierz wpis:\n\n";
        sf::String listText = sf::String::fromUtf8(header.begin(), header.end());

        for (size_t i = 0; i < loreEntries.size(); ++i) {
            std::string line;
            if (static_cast<int>(i) == selectedLoreIndex) {
                line = "> [LOG " + std::to_string(loreEntries[i].id) + "] " + loreEntries[i].title + "\n\n";
            } else {
                line = "  [LOG " + std::to_string(loreEntries[i].id) + "] " + loreEntries[i].title + "\n\n";
            }
            listText += sf::String::fromUtf8(line.begin(), line.end());
        }

        if (contentText.has_value()) {
            contentText->setString(listText);
            contentText->setCharacterSize(30);
        }
    } else {
        // Wyświetl treść wybranego logu
        if (selectedLoreIndex >= 0 && selectedLoreIndex < static_cast<int>(loreEntries.size())) {
            const LoreEntry& entry = loreEntries[selectedLoreIndex];
            std::string displayText = "[LOG " + std::to_string(entry.id) + "]\n";
            displayText += entry.title + "\n\n";
            displayText += entry.content;

            if (contentText.has_value()) {
                contentText->setString(sf::String::fromUtf8(displayText.begin(), displayText.end()));
                contentText->setCharacterSize(28);
            }
        }
    }
}

