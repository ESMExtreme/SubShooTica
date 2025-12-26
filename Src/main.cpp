#include <SFML/Graphics.hpp>
#include "MainMenu.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "SubShootica");
    MainMenu menu(window.getSize().x, window.getSize().y);

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
            }
        }

        window.clear();
        menu.draw(window);
        window.display();
    }

    return 0;
}