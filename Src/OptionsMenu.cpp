#include "OptionsMenu.h"
#include <fstream>
#include <sstream>

std::vector<sf::Vector2u> resolutions = { {1280,720}, {1920,1080}, {2560,1440}, {3840,2160} };
std::vector<std::string> fpsLimits = { "30","60","120","144","240","Unlimited" };
std::vector<std::string> VsyncOptions = { "ON","OFF" };
std::vector<std::string> fullscreenOptions = { "ON","OFF" };

struct  {
    int musicVolume = 50;
    int resolutionIndex = 1;
    int fullscreenIndex = 0;
    int vsyncIndex = 0;
    int fpsLimitIndex = 2;
} tempOptions;

void OptionsMenu::keyPressHandler(const sf::Event::KeyPressed* key, int* currentScreen)
{
    using Key = sf::Keyboard::Key;
    if (key->code == Key::Up) {
        MoveUp();
        return;
    }
    if (key->code == Key::Down) {
        MoveDown();
        return;
    }
    if (key->code == Key::Left) {
        switch (selectedItemIndex) {
        case 0:
            tempOptions.musicVolume -= 10;
            if (tempOptions.musicVolume < 0)
                tempOptions.musicVolume = 0;
            optionsMenu[0].setString(std::string("Music volume: ") + std::to_string(tempOptions.musicVolume));
            if (musicPtr) musicPtr->setVolume(static_cast<float>(tempOptions.musicVolume));
            break;
        case 1:
            tempOptions.resolutionIndex--;
            if (tempOptions.resolutionIndex < 0)
                tempOptions.resolutionIndex = static_cast<int>(resolutions.size()) - 1;
			optionsMenu[1].setString(std::string("Resolution: ") + std::to_string(resolutions[tempOptions.resolutionIndex].x) + "x" + std::to_string(resolutions[tempOptions.resolutionIndex].y));
            break;
        case 2:
            tempOptions.fullscreenIndex--;
            if (tempOptions.fullscreenIndex < 0)
                tempOptions.fullscreenIndex = static_cast<int>(fullscreenOptions.size()) - 1;
			optionsMenu[2].setString(std::string("Fullscreen: ") + fullscreenOptions[tempOptions.fullscreenIndex]);
            break;
        case 3:
            tempOptions.vsyncIndex--;
            if (tempOptions.vsyncIndex < 0)
                tempOptions.vsyncIndex = static_cast<int>(VsyncOptions.size()) - 1;
			optionsMenu[3].setString(std::string("Vsync: ") + VsyncOptions[tempOptions.vsyncIndex]);
            break;
        case 4:
            tempOptions.fpsLimitIndex--;
            if (tempOptions.fpsLimitIndex < 0)
                tempOptions.fpsLimitIndex = static_cast<int>(fpsLimits.size()) - 1;
			optionsMenu[4].setString(std::string("FPS Cap: ") + fpsLimits[tempOptions.fpsLimitIndex]);
            break;
        default:
            break;
        }
    }
    if (key->code == Key::Right) {
        switch (selectedItemIndex) {
        case 0:
            tempOptions.musicVolume += 10;
            if (tempOptions.musicVolume > 100)
                tempOptions.musicVolume = 100;
            optionsMenu[0].setString(std::string("Music volume: ") + std::to_string(tempOptions.musicVolume));
            if (musicPtr) musicPtr->setVolume(static_cast<float>(tempOptions.musicVolume));
            break;
        case 1:
            tempOptions.resolutionIndex++;
            if (tempOptions.resolutionIndex >= static_cast<int>(resolutions.size()))
                tempOptions.resolutionIndex = 0;
            optionsMenu[1].setString(std::string("Resolution: ") + std::to_string(resolutions[tempOptions.resolutionIndex].x) + "x" + std::to_string(resolutions[tempOptions.resolutionIndex].y));
            break;
        case 2:
            tempOptions.fullscreenIndex++;
            if (tempOptions.fullscreenIndex >= static_cast<int>(fullscreenOptions.size()))
                tempOptions.fullscreenIndex = 0;
            optionsMenu[2].setString(std::string("Fullscreen: ") + fullscreenOptions[tempOptions.fullscreenIndex]);
            break;
        case 3:
            tempOptions.vsyncIndex++;
            if (tempOptions.vsyncIndex >= static_cast<int>(VsyncOptions.size()))
                tempOptions.vsyncIndex = 0;
            optionsMenu[3].setString(std::string("Vsync: ") + VsyncOptions[tempOptions.vsyncIndex]);
            break;
        case 4:
            tempOptions.fpsLimitIndex++;
            if (tempOptions.fpsLimitIndex >= static_cast<int>(fpsLimits.size()))
                tempOptions.fpsLimitIndex = 0;
            optionsMenu[4].setString(std::string("FPS Cap: ") + fpsLimits[tempOptions.fpsLimitIndex]);
            break;
        default:
            break;
        }
    }
    if (key->code == Key::Enter) {
        switch (selectedItemIndex) {
        case 5:
            // Apply - zapisz ustawienia
            optionsList.resolution = resolutions[tempOptions.resolutionIndex];
            optionsList.fullscreen = (tempOptions.fullscreenIndex == 0) ? 1 : 0;
            optionsList.vsync = (tempOptions.vsyncIndex == 0) ? 1 : 0;
            optionsList.musicVolume = tempOptions.musicVolume;
            if (tempOptions.fpsLimitIndex == 5)
                optionsList.fpsLimit = 0;
            else
                optionsList.fpsLimit = std::stoi(fpsLimits[tempOptions.fpsLimitIndex]);
            {
                std::fstream zapis;
                zapis.open("Assets/options.txt", std::ios::out);
                zapis << "resolutionW:" << optionsList.resolution.x << std::endl;
                zapis << "resolutionH:" << optionsList.resolution.y << std::endl;
                zapis << "fullscreen:" << optionsList.fullscreen << std::endl;
                zapis << "vsync:" << optionsList.vsync << std::endl;
                zapis << "fps_limit:" << optionsList.fpsLimit << std::endl;
                zapis << "music_volume:" << optionsList.musicVolume << std::endl;
                zapis.close();
            }
            shouldApplySettings = true; // Ustaw flagę dla main.cpp
            break;
        case 6:
            *currentScreen = 0;
            break;
        default:
            break;
        }
    }
}

OptionsMenu::OptionsMenu(sf::Music* music) : musicPtr(music)
{
    // Załaduj tło background.png
    if (backgroundTexture.loadFromFile("Assets/Media/background.png")) {
        backgroundSprite.emplace(backgroundTexture);
    }

    // Załaduj teksturę opcje_box.png
    if (boxTexture.loadFromFile("Assets/Media/opcje_box.png")) {
        // Konfiguracja 1 boxa - pozycja i skala (możesz dostosować)
        boxConfigs = {
            {{300.f, 400.f}, {2.0f, 2.0f}}   // Box 0 - pojedynczy box
        };

        // Utwórz 1 sprite dla boxa
        for (size_t i = 0; i < 1; ++i) {
            sf::Sprite boxSprite(boxTexture);
            boxSprite.setPosition(boxConfigs[i].position);
            boxSprite.setScale(boxConfigs[i].scale);
            boxSprites.push_back(boxSprite);
        }
    }

    std::fstream optionsFile;
    optionsFile.open("Assets/options.txt", std::ios::in);
    if (!optionsFile.good()) {
        exit(0);
    }
    std::string linia;
    while (std::getline(optionsFile, linia)) {
        std::stringstream ss(linia);
        std::string key, value;
        std::getline(ss, key, ':');
        std::getline(ss, value);
        if (key == "resolutionW") {
            optionsList.resolution.x = std::stoi(value);
            if (value == "1920")
                tempOptions.resolutionIndex = 1;
            else if (value == "1280")
                tempOptions.resolutionIndex = 0;
            else if (value == "2560")
                tempOptions.resolutionIndex = 2;
            else if (value == "3840")
                tempOptions.resolutionIndex = 3;
        }
        else if (key == "resolutionH") {
            optionsList.resolution.y = std::stoi(value);
        }
        else if (key == "fullscreen") {
            optionsList.fullscreen = std::stoi(value);
            tempOptions.fullscreenIndex = (value == "0") ? 0 : 1;
        }
        else if (key == "vsync") {
            optionsList.vsync = std::stoi(value);
            tempOptions.vsyncIndex = (value == "0") ? 0 : 1;
        }
        else if (key == "fps_limit") {
            optionsList.fpsLimit = std::stoi(value);
            if (value == "30")
                tempOptions.fpsLimitIndex = 0;
            else if (value == "60")
                tempOptions.fpsLimitIndex = 1;
            else if (value == "120")
                tempOptions.fpsLimitIndex = 2;
            else if (value == "144")
                tempOptions.fpsLimitIndex = 3;
            else if (value == "240")
                tempOptions.fpsLimitIndex = 4;
            else if (value == "0")
                tempOptions.fpsLimitIndex = 5;
        }
        else if (key == "music_volume") {
            optionsList.musicVolume = std::stoi(value);
            tempOptions.musicVolume = optionsList.musicVolume;
        }
    }

    if (!font.openFromFile("Assets/Fonts/BerlinSans.ttf")) {
        // Nie udało się załadować czcionki - kontynuuj z domyślną
    }

    CreateTile(std::string("Music volume: ") + std::to_string(tempOptions.musicVolume), sf::Color(255, 165, 0), {450,500}, 40);
    CreateTile(std::string("Resolution: ") + std::to_string(resolutions[tempOptions.resolutionIndex].x)+"x"+ std::to_string(resolutions[tempOptions.resolutionIndex].y), sf::Color::White, {450,600}, 40);
    CreateTile(std::string("Fullscreen: ") + fullscreenOptions[tempOptions.fullscreenIndex], sf::Color::White, {450,700}, 40);
    CreateTile(std::string("Vsync: ") + VsyncOptions[tempOptions.vsyncIndex], sf::Color::White, { 950,500 }, 40);
    CreateTile(std::string("FPS Cap: ") + fpsLimits[tempOptions.fpsLimitIndex], sf::Color::White, { 950,600 }, 40);
	CreateTile(std::string("Apply"), sf::Color::White, { 950,700 }, 40);
    CreateTile(std::string("Back"), sf::Color::White, { 800,750 }, 40);
    selectedItemIndex = 0;
}

void OptionsMenu::CreateTile(std::string string, sf::Color color, sf::Vector2f position, int fontSize)
{
    sf::Text text(font);
    text.setString(string);
    text.setCharacterSize(fontSize);
    text.setFillColor(color);
    text.setPosition(position);
    optionsMenu.push_back(text);
}

void OptionsMenu::draw(sf::RenderWindow& window)
{
    // Rysuj tło background.png
    if (backgroundSprite.has_value()) {
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u textureSize = backgroundTexture.getSize();
        float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
        float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);
        backgroundSprite->setScale(sf::Vector2f(scaleX, scaleY));
        window.draw(*backgroundSprite);
    }

    // Rysuj 1 obrazek opcje_box.png
    for (const auto& boxSprite : boxSprites) {
        window.draw(boxSprite);
    }

    // Rysuj tekst opcji
    for (const auto& item : optionsMenu) {
        window.draw(item);
    }
}

void OptionsMenu::MoveUp() {
    optionsMenu[selectedItemIndex].setFillColor(sf::Color::White);
    if (selectedItemIndex == 0)
        selectedItemIndex = static_cast<int>(optionsMenu.size()) - 1;
    else
        --selectedItemIndex;
    optionsMenu[selectedItemIndex].setFillColor(sf::Color(255, 165, 0));
}

void OptionsMenu::MoveDown()
{
    optionsMenu[selectedItemIndex].setFillColor(sf::Color::White);
    if (selectedItemIndex == static_cast<int>(optionsMenu.size()) - 1)
        selectedItemIndex = 0;
    else
        ++selectedItemIndex;
    optionsMenu[selectedItemIndex].setFillColor(sf::Color(255, 165, 0));
}





