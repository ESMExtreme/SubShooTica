#include "MainMenu.h"
#include <SFML/Audio.hpp>
#include <fstream>



using namespace sf;
using namespace std;


MainMenu::MainMenu(float width, float height, sf::Music& music) : menuMusic(music)
{
    // ensure the font path matches what you copy in CMake (example: Assets/Fonts/arial.ttf)
    if (!font.openFromFile("Assets/Fonts/BerlinSans.ttf"))
    {
        // handle error (log, throw, fallback). For now we leave an inline comment.
    }

    if (!menuMusic.openFromFile("Assets/Audio/Subnautica_MainTheme.ogg")) {
        // std::cout << "Błąd ładowania muzyki" << std::endl;   
         std::ofstream log("log.txt", std::ios::app); // w build/bin jest log.txt
        log << "Blad ladowania muzyki" << std::endl;
    } else {
    menuMusic.setLooping(true);
    menuMusic.play();
    std::ofstream log("log.txt", std::ios::app); // w build/bin jest log.txt
    log << "Muzyka załadowana OK" << std::endl;

    
}
 

	CreateTile("Start Game", sf::Color::Red, { 400,200 }, 0);
    mainMenu[0].setScale({ 1.1f,1.1f });
    CreateTile("Options", sf::Color::White, { 400,400 }, 1);
    CreateTile("Exit", sf::Color::White, { 400,600 }, 2);
    selectedItemIndex = 0; // start with first item selected
}

void MainMenu::MoveUp() {
    mainMenu[selectedItemIndex].setFillColor(sf::Color::White);
    mainMenu[selectedItemIndex].setScale({ 1.0f,1.0f });
    if (selectedItemIndex == 0)
        selectedItemIndex = static_cast<int>(mainMenu.size()) - 1;
    else
        --selectedItemIndex;
        mainMenu[selectedItemIndex].setFillColor(sf::Color::Red);
        mainMenu[selectedItemIndex].setScale({ 1.1f,1.1f });
}

void MainMenu::MoveDown()
{
    mainMenu[selectedItemIndex].setFillColor(sf::Color::White);
    mainMenu[selectedItemIndex].setScale({ 1.0f,1.0f });
    if (selectedItemIndex == static_cast<int>(mainMenu.size()) - 1)
        selectedItemIndex = 0;
    else
        ++selectedItemIndex;
    mainMenu[selectedItemIndex].setFillColor(sf::Color::Red);
    mainMenu[selectedItemIndex].setScale({ 1.1f,1.1f });
}

void MainMenu::CreateTile(std::string string, sf::Color color, sf::Vector2f position,int index)
{
	sf::Text text(font);
	text.setString(string);
	text.setCharacterSize(70);
	text.setFillColor(color);
	text.setPosition(position);
	mainMenu.push_back(text);
}
void MainMenu::keyPressHandler(const Event::KeyPressed* key, int* currentScreen)
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
    if (key->code == Key::Enter )
    {
        switch (selectedItemIndex)
        {
        case 0:
            // Start Game selected
            *currentScreen = 2; // Zmień CurrentScreen na 2
            break;
        case 1:
            // Options selected
            *currentScreen = 1; // Switch to Options Menu
            break;
        case 2:
            // Exit selected
            exit(0);
            break;
        default:
            break;
		}
    }
}
void MainMenu::draw(sf::RenderWindow& window)
{
    for (const auto item : mainMenu)
    {
        window.draw(item);
    }
}

