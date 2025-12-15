#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <vector>

using namespace std;
int main()
{
    constexpr unsigned WINDOW_W = 1920u;
    constexpr unsigned WINDOW_H = 1080u;
    auto window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "SubShootica");
    window.setFramerateLimit(144);
    // berlin-sans-fb-demi-bold
    sf::Font font("BerlinSans.ttf"); // Throws sf::Exception if an error occurs
    // Main text (created once)
    sf::Text mainText(font);
    mainText.setString("Hello world");
    mainText.setCharacterSize(24);
    mainText.setFillColor(sf::Color(255, 165, 0)); // orange

    // Button rectangle
    sf::Vector2f buttonSize{ 200.f, 60.f };
    sf::RectangleShape button(buttonSize);
    button.setFillColor(sf::Color(70, 70, 70));
    button.setOutlineColor(sf::Color::White);
    button.setOutlineThickness(2.f);
    button.setPosition({ (1920.f - buttonSize.x) / 2.f, (1080.f - buttonSize.y) / 2.f });
    vector<float> buttonLocation{
        (((1920.f - buttonSize.x) / 2.f)),
        (((1920.f - buttonSize.x) / 2.f) + (buttonSize.x)),
        (((1080.f - buttonSize.y) / 2.f)),
        (((1080.f - buttonSize.y) / 2.f) + (buttonSize.y)),
};

    // Button label
    sf::Text buttonLabel(font);
    buttonLabel.setString("Change text");
    buttonLabel.setCharacterSize(20);
    buttonLabel.setFillColor(sf::Color::White);
    buttonLabel.setPosition({ (1920.f - buttonSize.x/2) / 2.f, (1080.f - buttonSize.y/2) / 2.f });
    vector<string> messages{
    "Hello world",
    "SubShootica!",
    "Button clicked",
    "SFML + CMake"
    };

    
    while (window.isOpen())
    {

        // Use `auto` so the compiler deduces the std::optional<sf::Event> type
        while (auto event = window.pollEvent())
        {
            const auto onClose = [&window](const sf::Event::Closed&)
                {
                    window.close();
                };

            const auto onKeyPressed = [&window](const sf::Event::KeyPressed& keyPressed)
                {

                    if (keyPressed.scancode == sf::Keyboard::Scancode::Escape)
                        cout << "Key pressed: " << endl;
                        window.close();

                };
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    cout << "the right button was pressed" << endl;
                    cout << "mouse x: " << mouseButtonPressed->position.x << ", button x:" << buttonLocation[0] << ","<<buttonLocation[1] << ", button y:" << buttonLocation[2] << "," << buttonLocation[3] << endl;
                    cout << "mouse y: " << mouseButtonPressed->position.y << endl;
                    if ((mouseButtonPressed->position.x <= buttonLocation[1])&& (mouseButtonPressed->position.y >= buttonLocation[2])&& (mouseButtonPressed->position.y <= buttonLocation[3])) {
                        buttonLabel.setString("bubek");
                    }
                }
            }
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Clear the window with a blue background
        window.clear(sf::Color::Cyan);
        window.draw(mainText);
        window.draw(button);
        window.draw(buttonLabel);
        window.display();
    }
}
