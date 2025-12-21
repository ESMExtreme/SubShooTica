#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include <iostream>
#include <vector>
#include "MainMenu.h"
#include "OptionsMenu.h"

struct
{
    unsigned WINDOW_W = 1920u;
    unsigned WINDOW_H = 1080u;
    int FramerateLimit = 144;
    bool mainMenuOn = true;
    bool optionsMenuOn = false;
} CurrentlyRendering;
using namespace std;





int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({ CurrentlyRendering.WINDOW_W, CurrentlyRendering.WINDOW_H }), "SubShootica");
    window.setFramerateLimit(CurrentlyRendering.FramerateLimit);

    sf::Vector2u size = window.getSize();
    auto [Wwidth, Wheight] = size;

    while (window.isOpen())
    {


        if (CurrentlyRendering.mainMenuOn) {
            MainMenuRenderer(window, &CurrentlyRendering.mainMenuOn, &CurrentlyRendering.optionsMenuOn);
        }
        if (CurrentlyRendering.optionsMenuOn) {
            OptionsMenuRenderer(window, &CurrentlyRendering.mainMenuOn, &CurrentlyRendering.optionsMenuOn, &CurrentlyRendering.WINDOW_W, &CurrentlyRendering.WINDOW_H, &CurrentlyRendering.FramerateLimit);
        }

    }

}
