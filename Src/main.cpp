#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include <iostream>
#include <vector>

using namespace std;
int main()
{
    constexpr unsigned WINDOW_W = 1920u;
    constexpr unsigned WINDOW_H = 1080u;
    auto window = sf::RenderWindow(sf::VideoMode({ WINDOW_W, WINDOW_H }), "SubShootica");
    window.setFramerateLimit(144);

    sf::Vector2u size = window.getSize();
    auto [Wwidth, Wheight] = size;


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
    button.setPosition({ (Wwidth - buttonSize.x) / 2.f, (Wheight - buttonSize.y) / 2.f });
    vector<float> buttonLocation{
        (((Wwidth - buttonSize.x) / 2.f)),
        (((Wwidth - buttonSize.x) / 2.f) + (buttonSize.x)),
        (((Wheight - buttonSize.y) / 2.f)),
        (((Wheight - buttonSize.y) / 2.f) + (buttonSize.y)),
};

    // Button label
    sf::Text buttonLabel(font);
    buttonLabel.setString("Change text");
    buttonLabel.setCharacterSize(20);
    buttonLabel.setFillColor(sf::Color::White);
    buttonLabel.setPosition({ (Wwidth - buttonSize.x/2) / 2.f, (Wheight - buttonSize.y/2) / 2.f });
    vector<string> messages{
    "Hello world",
    "SubShootica!",
    "Button clicked",
    "SFML + CMake"
    };
	int LabelI = 0;

    
    while (window.isOpen())
    {

        // Use `auto` so the compiler deduces the std::optional<sf::Event> type
        while (auto event = window.pollEvent())
        {
            const auto onClose = [&window](const sf::Event::Closed&)
                {
                    window.close();
                };


            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {

                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    cout << "mouse x: " << mouseButtonPressed->position.x << ", mouse y: " << mouseButtonPressed->position.y << endl;
                    if ((mouseButtonPressed->position.x <= buttonLocation[1]) && (mouseButtonPressed->position.y >= buttonLocation[2]) && (mouseButtonPressed->position.y <= buttonLocation[3])) {
                        LabelI++;
                        if (LabelI >= messages.size()) {
                            LabelI = 0;
						}
                        buttonLabel.setString(messages[LabelI]);
                    }
                }
            }


            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Real-time keyboard handling (checks current state; good for continuous movement)
        if (window.hasFocus())
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                cout << "Right key pressed (realtime)" << endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                cout << "Left key pressed (realtime)" << endl;
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
