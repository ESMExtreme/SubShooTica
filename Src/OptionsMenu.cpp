#include <SFML/Audio.hpp>
#include "OptionsMenu.h"



using namespace sf;
using namespace std;
vector<Vector2u> resolutions = { {1280,720}, {1920,1080}, {2560,1440}, {3840,2160} };
vector<string> fpsLimits = { "30","60","120","144","240","Unlimited" };
vector<string> VsyncOptions = { "ON","OFF" };
vector<string> fullscreenOptions = { "ON","OFF" };

struct  {
    int musicVolume = 50;
    int resolutionIndex = 1;
    int fullscreenIndex = 0;
    int vsyncIndex = 0;
    int fpsLimitIndex = 2;
} tempOptions;
void OptionsMenu::keyPressHandler(const Event::KeyPressed* key, int* currentScreen)
{
    using Key = sf::Keyboard::Key;
    if (key->code == Key::Up)
    {
        MoveUp();
        return;
    }

    if (key->code == Key::Down)
    {
        MoveDown();
        return;
    }
    if (key->code == Key::Left)
    {
        switch (selectedItemIndex)
        {
        case 0: 
            cout << "Left pressed on volume bar" << endl;
            tempOptions.musicVolume -= 10;
            if (tempOptions.musicVolume < 0)
                tempOptions.musicVolume = 0;
            optionsMenu[0].setString(string("Music volume: ") + to_string(tempOptions.musicVolume));
            if (musicPtr) musicPtr->setVolume(static_cast<float>(tempOptions.musicVolume));
            break;
        case 1:
            cout << "Left pressed on Resolution" << endl;
            tempOptions.resolutionIndex--;
            if (tempOptions.resolutionIndex < 0)
                tempOptions.resolutionIndex = static_cast<int>(resolutions.size()) - 1;
			optionsMenu[1].setString(string("Resolution: ") + to_string(resolutions[tempOptions.resolutionIndex].x) + "x" + to_string(resolutions[tempOptions.resolutionIndex].y));
            break;
        case 2:
            tempOptions.fullscreenIndex--;
            if (tempOptions.fullscreenIndex < 0)
                tempOptions.fullscreenIndex = static_cast<int>(fullscreenOptions.size()) - 1;
			optionsMenu[2].setString(string("Fullscreen: ") + fullscreenOptions[tempOptions.fullscreenIndex]);
            cout << "Left pressed on Screen" << endl;
            break;
        case 3:
            tempOptions.vsyncIndex--;
            if (tempOptions.vsyncIndex < 0)
                tempOptions.vsyncIndex = static_cast<int>(VsyncOptions.size()) - 1;
			optionsMenu[3].setString(string("Vsync: ") + VsyncOptions[tempOptions.vsyncIndex]);
            cout << "Left pressed on VSync" << endl;
            break;
        case 4:
            tempOptions.fpsLimitIndex--;
            if (tempOptions.fpsLimitIndex < 0)
                tempOptions.fpsLimitIndex = static_cast<int>(fpsLimits.size()) - 1;
			optionsMenu[4].setString(string("FPS Cap: ") + fpsLimits[tempOptions.fpsLimitIndex]);
            cout << "Left pressed on FPS Cap" << endl;
            break;
        default:
            break;

        }
    }

    if (key->code == Key::Right)
    {
        switch (selectedItemIndex)
        {
        case 0:
            cout << "Right pressed on volume bar" << endl;
            tempOptions.musicVolume += 10;
            if (tempOptions.musicVolume > 100)
                tempOptions.musicVolume = 100;
            optionsMenu[0].setString(string("Music volume: ") + to_string(tempOptions.musicVolume));
            if (musicPtr) musicPtr->setVolume(static_cast<float>(tempOptions.musicVolume));
            break;
        case 1:
            tempOptions.resolutionIndex++;
            if (tempOptions.resolutionIndex >= static_cast<int>(resolutions.size()))
                tempOptions.resolutionIndex = 0;
            optionsMenu[1].setString(string("Resolution: ") + to_string(resolutions[tempOptions.resolutionIndex].x) + "x" + to_string(resolutions[tempOptions.resolutionIndex].y));
            cout << "Left pressed on Resolution" << endl;
            break;
        case 2:
            tempOptions.fullscreenIndex++;
            if (tempOptions.fullscreenIndex >= static_cast<int>(fullscreenOptions.size()))
                tempOptions.fullscreenIndex = 0;
            optionsMenu[2].setString(string("Fullscreen: ") + fullscreenOptions[tempOptions.fullscreenIndex]);
            cout << "Left pressed on Screen" << endl;
            break;
        case 3:
            tempOptions.vsyncIndex++;
            if (tempOptions.vsyncIndex >= static_cast<int>(VsyncOptions.size()))
                tempOptions.vsyncIndex = 0;
            optionsMenu[3].setString(string("Vsync: ") + VsyncOptions[tempOptions.vsyncIndex]);
            cout << "Left pressed on VSync" << endl;
            break;
        case 4:
            tempOptions.fpsLimitIndex++;
            if (tempOptions.fpsLimitIndex >= static_cast<int>(fpsLimits.size()))
                tempOptions.fpsLimitIndex = 0;
            optionsMenu[4].setString(string("FPS Cap: ") + fpsLimits[tempOptions.fpsLimitIndex]);
            cout << "Left pressed on FPS Cap" << endl;
            break;
        default:
            break;

        }
    }
    if (key->code == Key::Enter)
    {
        switch (selectedItemIndex)
        {
        case 5:
            // Apply selected
            cout << "Apply selected" << endl;
            optionsList.resolution = resolutions[tempOptions.resolutionIndex];
            optionsList.fullscreen = (tempOptions.fullscreenIndex == 0) ? 1 : 0;
            optionsList.vsync = (tempOptions.vsyncIndex == 0) ? 1 : 0;
            optionsList.musicVolume = tempOptions.musicVolume;
            if (tempOptions.fpsLimitIndex == 5)
                optionsList.fpsLimit = 0;
            else
                optionsList.fpsLimit = stoi(fpsLimits[tempOptions.fpsLimitIndex]);
            {
                fstream zapis;
                zapis.open("Assets/options.txt", ios::out);
                zapis << "resolutionW:" << optionsList.resolution.x << endl;
                zapis << "resolutionH:" << optionsList.resolution.y << endl;
                zapis << "fullscreen:" << optionsList.fullscreen << endl;
                zapis << "vsync:" << optionsList.vsync << endl;
                zapis << "fps_limit:" << optionsList.fpsLimit << endl;
                zapis << "music_volume:" << optionsList.musicVolume << endl;
                zapis.close();
            }

            break;
        case 6:
            // Exit selected
            *currentScreen = 0; // Switch to Options Menu
            break;
        }
    }
}
OptionsMenu::OptionsMenu(sf::Music* music) : musicPtr(music)
{
    fstream optionsFile; //zmienna plikowa, okre�lenie pliku, tzw. uchwyt do pliku
    optionsFile.open("Assets/options.txt", ios::in); // funkcja do otwarcia pliku, �cie�ka pliku, tryb otwarcia ios::in->odczyt z pliku
    if (optionsFile.good() == false)//zwraca warto�� prawda/fa�sz w zale�no�ci od tego czy istnieje plik
    {
        cout << "Brak pliku" << endl;
        exit(0);
    }
    string linia;
    string word;
    while (getline(optionsFile, linia)) {
        stringstream ss(linia);
        string key, value;
        getline(ss, key, ':');   // "resolution"
        getline(ss, value);     // " 1920x1080"
        cout << key << " -> " << value << endl;
        if (key == "resolutionW") {
            optionsList.resolution.x = stoi(value);
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
            optionsList.resolution.y = stoi(value);
        }
        else if (key == "fullscreen") {
            optionsList.fullscreen = stoi(value);
            if (value == "0")
                tempOptions.fullscreenIndex = 0;

            else
                tempOptions.fullscreenIndex = 1;
        }
        else if (key == "vsync") {
            optionsList.vsync = stoi(value);
            if (value == "0")
                tempOptions.vsyncIndex = 0;
            else
                tempOptions.vsyncIndex = 1;
        }
        else if (key == "fps_limit") {
            optionsList.fpsLimit = stoi(value);
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
            optionsList.musicVolume = stoi(value);
            tempOptions.musicVolume = optionsList.musicVolume;
        }

    };

    // ensure the font path matches what you copy in CMake (example: Assets/Fonts/arial.ttf)
    if (!font.openFromFile("Assets/Fonts/BerlinSans.ttf"))
    {
        // handle error (log, throw, fallback). For now we leave an inline comment.
    }
    CreateTile(string("Music volume: ") + to_string(tempOptions.musicVolume), sf::Color::Red, {400,100}, 0, 70);
    CreateTile(string("Resolution: ") + to_string(resolutions[tempOptions.resolutionIndex].x)+"x"+ to_string(resolutions[tempOptions.resolutionIndex].y), sf::Color::White, {400,200}, 0, 70);
    CreateTile(std::string("Fullscreen: ") + fullscreenOptions[tempOptions.fullscreenIndex], sf::Color::White, {400,300}, 1, 70);
    CreateTile(std::string("Vsync: ") + VsyncOptions[tempOptions.vsyncIndex], sf::Color::White, { 400,400 }, 1, 70);
    CreateTile(string("FPS Cap: ") + fpsLimits[tempOptions.fpsLimitIndex], sf::Color::White, { 400,500 }, 0, 70);
	CreateTile(std::string("Apply"), sf::Color::White, { 400,600 }, 3, 70);
    CreateTile(std::string("Back"), sf::Color::White, { 700,700 }, 3, 70);
    selectedItemIndex = 0; // start with first item selected
}
void OptionsMenu::CreateTile(std::string string, sf::Color color, sf::Vector2f position, int index,int fontSize)
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
    for (const auto item : optionsMenu)
    {
        window.draw(item);
    }
}

void OptionsMenu::MoveUp() {
    optionsMenu[selectedItemIndex].setFillColor(sf::Color::White);
    if (selectedItemIndex == 0)
        selectedItemIndex = static_cast<int>(optionsMenu.size()) - 1;
    else
        --selectedItemIndex;
    optionsMenu[selectedItemIndex].setFillColor(sf::Color::Red);
}

void OptionsMenu::MoveDown()
{
    optionsMenu[selectedItemIndex].setFillColor(sf::Color::White);
    if (selectedItemIndex == static_cast<int>(optionsMenu.size()) - 1)
        selectedItemIndex = 0;
    else
        ++selectedItemIndex;
    optionsMenu[selectedItemIndex].setFillColor(sf::Color::Red);
}
