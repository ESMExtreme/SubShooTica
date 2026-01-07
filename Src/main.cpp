#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "SaveScreen.h"

using namespace sf;
using namespace std;
int CurrentScreen = 0; // 0 - MainMenu, 1 - OptionsMenu , 2 - SaveScreen

int main()
{
    sf::RenderWindow window;
	OptionsMenu optionsMenu;
    SaveScreen saveScreen;

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
        // SFML 3: pollEvent() returns std::optional<sf::Event>
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                switch (CurrentScreen) {
                    case 0:
                        menu.keyPressHandler(key, &CurrentScreen);
                        break;
                    case 1:
                        optionsMenu.keyPressHandler(key, &CurrentScreen);
                        break;
                    case 2:
                        saveScreen.keyPressHandler(key, &CurrentScreen);
                        break;
                }

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
		}
        window.display();
    }

    return 0;
}