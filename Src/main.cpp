#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include "GameScreen.h"
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "SaveScreen.h"
#include "GameMap.h"
#include "ShopInGame.h"

using namespace sf;
using namespace std;
int CurrentScreen = 0; // 0 - MainMenu, 1 - OptionsMenu , 2 - SaveScreen , 3-Mapa, 4-GameScreen
int PreviousScreen = 0; // Śledź poprzedni ekran

int main()
{
    sf::RenderWindow window;
    sf::Music menuMusic;

	OptionsMenu optionsMenu(&menuMusic);
    SaveScreen saveScreen;
    ShopInGame shop;
    GameScreen lvl1;
    GameMap map;

    sf::Clock clock; 

    menuMusic.setVolume(static_cast<float>(optionsMenu.optionsList.musicVolume));

    if (optionsMenu.optionsList.fullscreen) {
        window.create(sf::VideoMode(optionsMenu.optionsList.resolution), "SubShootica", Style::None, State::Fullscreen);
    }
    else {
        window.create(sf::VideoMode(optionsMenu.optionsList.resolution), "SubShootica", Style::Close | Style::Titlebar, State::Windowed);
	}
    MainMenu menu(window.getSize().x, window.getSize().y, menuMusic);
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
                    map.handleEvent(*event, &CurrentScreen);
                    break;
                case 4:
                    shop.handleEvent(*event, &CurrentScreen);
                    break;
                case 5:
                    lvl1.handleEvent(*event, &CurrentScreen);
                    break;
            }
        }

        // Sprawdź czy zmienił się ekran i zaktualizuj slot zapisu
        if (PreviousScreen != CurrentScreen) {
            if (PreviousScreen == 2 && CurrentScreen == 3) {
                // Przejście z SaveScreen do GameMap
                int slot = saveScreen.getSelectedSaveSlot();
                map.setSaveSlot(slot);

            } else if (PreviousScreen == 3 && CurrentScreen == 4) {
                // Przejście z GameMap do ShopInGame (poziom 0)
                int slot = map.getSaveSlot();

                // KROK 1: Wczytaj dane gry (aby pobrać aktualny złom)
                lvl1.setSaveSlot(slot); // To wczyta save*.dat

                // KROK 2: Wczytaj dane sklepu
                shop.setSaveSlot(slot); // To wczyta shop*.dat

                // KROK 3: Synchronizuj złom z gry do sklepu
                shop.setPlayerScrap(lvl1.getScrap());

            } else if (PreviousScreen == 3 && CurrentScreen == 5) {
                // Przejście z GameMap do GameScreen (poziomy 1-12)
                int slot = map.getSaveSlot();
                lvl1.setSaveSlot(slot);
                lvl1.setLevel(map.getSelectedLevel());

            }
            PreviousScreen = CurrentScreen; // Zapisz nowy ekran
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
                map.draw(window);
                break;
            case 4:
                shop.draw(window);
                break;
             case 5:
                lvl1.update(deltaTime, window.getSize());
                lvl1.draw(window);
                break;
        }
        window.display();
    }

    return 0;
}