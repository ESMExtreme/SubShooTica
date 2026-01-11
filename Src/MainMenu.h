#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>



class MainMenu
{
    public:
    MainMenu(float width, float height, sf::Music& music);
    void draw(sf::RenderWindow& window);
    int GetPressedItem() const { return selectedItemIndex; }
    void CreateTile(std::string string, sf::Color color, sf::Vector2f position,int index);
	void keyPressHandler(const sf::Event::KeyPressed* key, int* currentScreen);
    
    private:
    sf::Music& menuMusic;
    sf::Font font;
    std::vector<sf::Text> mainMenu;
    int selectedItemIndex;
    void MoveUp();
    void MoveDown();
};
