#include "MainMenu.h"

MainMenu::MainMenu(float width, float height)
{
    // ensure the font path matches what you copy in CMake (example: Assets/Fonts/arial.ttf)
    if (!font.openFromFile("Assets/Fonts/arial.ttf"))
    {
        // handle error (log, throw, fallback). For now we leave an inline comment.
    }
	CreateTile("Start Game", sf::Color::White, { 400,200 }, 0);
    CreateTile("Options", sf::Color::Red, { 400,400 }, 1);
    CreateTile("GGs Game", sf::Color::White, { 400,600 }, 2);
    selectedItemIndex = 0; // start with first item selected
}
void MainMenu::CreateTile(std::string string, sf::Color color, sf::Vector2f position,int index)
{
	sf::Text text(font);
	text.setString(string);
	text.setCharacterSize(70);
	text.setPosition(position);
	mainMenu.push_back(text);
}
void MainMenu::draw(sf::RenderWindow& window)
{
    for (const auto item : mainMenu)
    {
        window.draw(item);
    }
}

void MainMenu::MoveUp() {
    mainMenu[selectedItemIndex].setFillColor(sf::Color::White);
    if (selectedItemIndex == 0)
        selectedItemIndex = static_cast<int>(mainMenu.size()) - 1;
    else
        --selectedItemIndex;
	mainMenu[selectedItemIndex].setFillColor(sf::Color::Red);
}

void MainMenu::MoveDown()
{
    mainMenu[selectedItemIndex].setFillColor(sf::Color::White);
    if (selectedItemIndex == static_cast<int>(mainMenu.size()) - 1)
        selectedItemIndex = 0;
    else
        ++selectedItemIndex;
    mainMenu[selectedItemIndex].setFillColor(sf::Color::Red);
}