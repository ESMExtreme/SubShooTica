#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

class Menu { // Deklaracja klasy Menu
private:
    sf::Font font;
    sf::Text title01;
    sf::Text title02;

public:
    Menu(sf::RenderWindow& window);

    // Wyświetla menu i zwraca true jeśli gracz nacisnął ENTER
    bool run(sf::RenderWindow& window);
};

#endif
