#include <SFML/Graphics.hpp>
#include "Menu.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SubShooTica");
    window.setFramerateLimit(60);

    Menu menu(window);
    bool game_start = menu.run(window);

    if (!game_start) {
        return 0;
    }

    // Pętla gry
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        // tutaj dodasz logikę gry

        window.display();
    }

    return 0;
}
