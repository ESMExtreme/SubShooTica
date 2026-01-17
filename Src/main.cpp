#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include "Level1.h"
#include "Level2.h"
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "SaveScreen.h"
#include "GameMap.h"
#include "ShopInGame.h"

using namespace sf;
using namespace std;
int CurrentScreen = 0; // 0 - MainMenu, 1 - OptionsMenu , 2 - SaveScreen , 3-Mapa, 4-ShopInGame, 5-Poziomy
int PreviousScreen = 0; // Śledź poprzedni ekran
int currentLevel = 0; // Aktualnie wybrany poziom (0=sklep, 1=level1, 2=level2, itd.)

int main()
{
    sf::RenderWindow window;
    sf::Music menuMusic;

	OptionsMenu optionsMenu(&menuMusic);
    SaveScreen saveScreen;
    ShopInGame shop;
    Level1 lvl1;
    Level2 lvl2;
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
                    // Obsługa zdarzeń dla odpowiedniego poziomu
                    if (currentLevel == 1) {
                        lvl1.handleEvent(*event, &CurrentScreen);
                    } else if (currentLevel == 2) {
                        lvl2.handleEvent(*event, &CurrentScreen);
                    } else {
                        lvl1.handleEvent(*event, &CurrentScreen);
                    }
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

                // Wczytaj dane gry (zawiera złom i dane sklepu)
                lvl1.setSaveSlot(slot);

                // Ustaw slot i wczytaj dane sklepu z SaveData
                shop.setSaveSlot(slot);
                shop.loadFromSaveData(lvl1.getSaveData());

            } else if (PreviousScreen == 3 && CurrentScreen == 5) {
                // Przejście z GameMap do poziomów (1-12)
                int slot = map.getSaveSlot();
                currentLevel = map.getSelectedLevel();

                // Wybierz odpowiedni poziom
                if (currentLevel == 1) {
                    lvl1.setSaveSlot(slot);
                    lvl1.setLevel(currentLevel);
                } else if (currentLevel == 2) {
                    lvl2.setSaveSlot(slot);
                    lvl2.setLevel(currentLevel);
                } else {
                    // Domyślnie poziom 1 dla pozostałych (3-12 można dodać później)
                    lvl1.setSaveSlot(slot);
                    lvl1.setLevel(currentLevel);
                }

            }
            PreviousScreen = CurrentScreen; // Zapisz nowy ekran
        }

        // Sprawdź czy trzeba zastosować nowe ustawienia z OptionsMenu
        if (optionsMenu.shouldApplySettings) {
            optionsMenu.shouldApplySettings = false;

            // Zastosuj muzykę
            menuMusic.setVolume(static_cast<float>(optionsMenu.optionsList.musicVolume));

            // Zastosuj rozdzielczość i fullscreen
            if (optionsMenu.optionsList.fullscreen) {
                window.create(sf::VideoMode(optionsMenu.optionsList.resolution), "SubShootica", Style::None, State::Fullscreen);
            } else {
                window.create(sf::VideoMode(optionsMenu.optionsList.resolution), "SubShootica", Style::Close | Style::Titlebar, State::Windowed);
            }

            // Zastosuj VSync lub FPS limit
            if (optionsMenu.optionsList.vsync) {
                window.setVerticalSyncEnabled(true);
                window.setFramerateLimit(0); // Wyłącz limit FPS gdy VSync jest włączony
            } else {
                window.setVerticalSyncEnabled(false);
                window.setFramerateLimit(optionsMenu.optionsList.fpsLimit);
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
                map.draw(window);
                break;
            case 4:
                shop.draw(window);
                break;
             case 5:
                // Rysuj odpowiedni poziom
                if (currentLevel == 1) {
                    lvl1.update(deltaTime, window.getSize());
                    lvl1.draw(window);
                } else if (currentLevel == 2) {
                    lvl2.update(deltaTime, window.getSize());
                    lvl2.draw(window);
                } else {
                    lvl1.update(deltaTime, window.getSize());
                    lvl1.draw(window);
                }
                break;
        }
        window.display();
    }

    return 0;
}