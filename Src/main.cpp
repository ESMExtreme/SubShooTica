#include <SFML/Graphics.hpp>

#include "GameScreen.h"
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "SaveScreen.h"

using namespace sf;
using namespace std;
int CurrentScreen = 0; // 0 - MainMenu, 1 - OptionsMenu , 2 - SaveScreen , 3-Game

int main()
{
    sf::RenderWindow window;
	OptionsMenu optionsMenu;
    SaveScreen saveScreen;
    GameScreen game;
    sf::Clock clock;

    if (optionsMenu.optionsList.fullscreen) {
        window.create(sf::VideoMode(optionsMenu.optionsList.resolution), "SubShootica", Style::None, State::Fullscreen);
    }
    else {
        window.create(sf::VideoMode(optionsMenu.optionsList.resolution), "SubShootica", Style::Close | Style::Titlebar, State::Windowed);
	}
    MainMenu menu(window.getSize().x, window.getSize().y);
    if (optionsMenu.optionsList.vsync) {
        window.setVerticalSyncEnabled(true);
    }
    else {
        window.setFramerateLimit(optionsMenu.optionsList.fpsLimit);
	}

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        // SFML 3: pollEvent() returns std::optional<sf::Event>
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            switch (CurrentScreen) {
                case 0:
                    if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                        menu.keyPressHandler(key, &CurrentScreen);
                    }
                    break;
                case 1:
                    if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                        optionsMenu.keyPressHandler(key, &CurrentScreen);
                    }
                    break;
                case 2:
                    if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                        saveScreen.keyPressHandler(key, &CurrentScreen);
                    }
                    break;
                case 3:
                    game.handleEvent(*event, &CurrentScreen);
                    break;
            }
        }

        window.clear();
        switch (CurrentScreen) {
            case 0:
                menu.draw(window);
                break;
            case 1:
                optionsMenu.draw(window);
                break;
            case 2:
                saveScreen.draw(window);
                break;
            case 3:
                game.update(deltaTime, window.getSize());
                game.draw(window);
                break;
        }
        window.display();
    }

    return 0;
}