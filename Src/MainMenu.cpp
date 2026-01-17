#include "MainMenu.h"
#include <SFML/Audio.hpp>
#include <fstream>



using namespace sf;
using namespace std;


MainMenu::MainMenu(float width, float height, sf::Music& music) : menuMusic(music)
{
    // Załaduj tło background.png
    if (backgroundTexture.loadFromFile("Assets/Media/background.png")) {
        backgroundSprite.emplace(backgroundTexture);
        std::ofstream log("log.txt", std::ios::app);
        log << "Tło background.png załadowane pomyślnie" << std::endl;
    } else {
        std::ofstream log("log.txt", std::ios::app);
        log << "BŁĄD: Nie można załadować Assets/Media/background.png" << std::endl;
    }

    // Załaduj obrazek menu.png (na wierzchu tła)
    if (menuTexture.loadFromFile("Assets/Media/menu.png")) {
        menuSprite.emplace(menuTexture);
        std::ofstream log("log.txt", std::ios::app);
        log << "Obrazek menu.png załadowany pomyślnie" << std::endl;
    } else {
        std::ofstream log("log.txt", std::ios::app);
        log << "BŁĄD: Nie można załadować Assets/Media/menu.png" << std::endl;
    }

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
 

	CreateTile("Start Game", sf::Color::Red, { 455,535 }, 0);
    mainMenu[0].setScale({ 1.1f,1.1f });
    CreateTile("Options", sf::Color::White, { 455,640 }, 1);
    CreateTile("Exit", sf::Color::White, { 455,745 }, 2);
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
    // Rysuj tło (background.png)
    if (backgroundSprite.has_value()) {
        // Skaluj tło do rozmiaru okna
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u textureSize = backgroundTexture.getSize();
        float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
        float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);
        backgroundSprite->setScale(sf::Vector2f(scaleX, scaleY));
        window.draw(*backgroundSprite);
    }

    // Rysuj obrazek menu.png na wierzchu tła
    if (menuSprite.has_value()) {
        // Zmniejsz obrazek menu.png do 50% rozmiaru okna i wycentruj
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u menuTextureSize = menuTexture.getSize();

        // Skaluj do 50% rozmiaru okna (zmień 0.5f aby dostosować wielkość)
        float scaleX = (static_cast<float>(windowSize.x) / static_cast<float>(menuTextureSize.x)) * 0.3f;
        float scaleY = (static_cast<float>(windowSize.y) / static_cast<float>(menuTextureSize.y)) * 0.3f;
        menuSprite->setScale(sf::Vector2f(scaleX, scaleY));

        // Wycentruj obrazek na ekranie
        float posX = ((windowSize.x - menuTextureSize.x * scaleX) / 2.0f) - 350.0f;
        float posY = ((windowSize.y - menuTextureSize.y * scaleY) / 2.0f) + 150.0f;
        menuSprite->setPosition(sf::Vector2f(posX, posY));

        window.draw(*menuSprite);
    }

    // Rysuj elementy menu (tekst)
    for (const auto item : mainMenu)
    {
        window.draw(item);
    }
}

