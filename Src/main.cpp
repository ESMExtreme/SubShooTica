#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include <fstream>
#include <iostream> 
#include <string>
#include <sstream>

using namespace sf;
using namespace std;

class Options {
    public:
        sf::Vector2u resolution = { 1920, 1080 };
        bool fullscreen = true;
        bool vsync = true;
        int fpsLimit = 60;
};

int main()
{
	Options options;
    fstream optionsFile; //zmienna plikowa, okreœlenie pliku, tzw. uchwyt do pliku
    optionsFile.open("Assets/options.txt", ios::in); // funkcja do otwarcia pliku, œcie¿ka pliku, tryb otwarcia ios::in->odczyt z pliku
    if (optionsFile.good() == false)//zwraca wartoœæ prawda/fa³sz w zale¿noœci od tego czy istnieje plik
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
            options.resolution.x = stoi(value);

        }
        else if (key == "resolutionH") {
            options.resolution.y = stoi(value);
        }
        else if (key == "fullscreen") {
            options.fullscreen = stoi(value);
        }
        else if (key == "vsync") {
            options.vsync = stoi(value);
        }
        else if (key == "fps_limit") {
            options.fpsLimit = stoi(value);
        }

    }
    optionsFile.close();
    

    sf::RenderWindow window;
    if (options.fullscreen) {
        window.create(sf::VideoMode(options.resolution), "SubShootica", Style::None, State::Fullscreen);
    }
    else {
        window.create(sf::VideoMode(options.resolution), "SubShootica", Style::Close | Style::Titlebar, State::Windowed);
	}
    MainMenu menu(window.getSize().x, window.getSize().y);
    if (options.vsync) {
        window.setVerticalSyncEnabled(true);
    }
    else {
        window.setFramerateLimit(options.fpsLimit);
	}

    while (window.isOpen())
    {
        // SFML 3: pollEvent() returns std::optional<sf::Event>
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // SFML 3: use getIf<T>() to read specific event payloads
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Up)
                    menu.MoveUp();
                else if (key->code == sf::Keyboard::Key::Down)
                    menu.MoveDown();
                else if (key->code == sf::Keyboard::Key::Enter)
                {
                    int selectedItem = menu.GetPressedItem();
                    if (selectedItem == 0)
                    {   

                        // Start Game
                    }
                    else if (selectedItem == 1)
                    {
						RenderWindow optionsWindow(sf::VideoMode({ 400, 400 }), "Options", Style::None, State::Fullscreen);
                        // Options
						optionsWindow.display();
                        while (optionsWindow.isOpen())
                        {
                            while (auto oevent = optionsWindow.pollEvent())
                            {
                                if (oevent->is<sf::Event::Closed>())
                                    optionsWindow.close();
                                if (const auto* okey = oevent->getIf<sf::Event::KeyPressed>())
                                {
                                    if (okey->code == sf::Keyboard::Key::Escape)
                                        optionsWindow.close();
                                }
                            }
						}
                    }
                    else if (selectedItem == 2)
                    {
                        window.close(); // Exit game
                    }
				}
                    
            }
        }

        window.clear();
        menu.draw(window);
        window.display();
    }

    return 0;
}