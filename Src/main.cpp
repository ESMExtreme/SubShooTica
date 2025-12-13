#include <SFML/Graphics.hpp>
#include <optional>

using namespace std;
int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "SubShootica");
    window.setFramerateLimit(144);
    // berlin-sans-fb-demi-bold
    sf::Font font("BerlinSans.ttf"); // Throws sf::Exception if an error occurs

    while (window.isOpen())
    {
        // Use `auto` so the compiler deduces the std::optional<sf::Event> type
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Clear the window with a blue background
        window.clear();
        sf::Text text(font); // a font is required to make a text object

        // set the string to display
        text.setString("Hello world");

        // set the character size
        text.setCharacterSize(24); // in pixels, not points!

        // set the color
        text.setFillColor(sf::Color::Red);

        // set the text style
        text.setStyle(sf::Text::Bold | sf::Text::Underlined);

            // inside the main loop, between window.clear() and window.display()
            window.draw(text);
        window.display();
    }
}
