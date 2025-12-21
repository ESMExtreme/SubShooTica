#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include <iostream>
#include <vector>
#include "OptionsMenu.h"
using namespace std;
float margin = 15;
int LabelI1 = 0;
int LabelI2 = 0;
void OptionsMenuRenderer(sf::RenderWindow& window, bool* mainMenuOn, bool* optionsMenuOn, unsigned* WINDOW_W, unsigned* WINDOW_H, int* FramerateLimit) {

    sf::Vector2u size = window.getSize();
    auto [Wwidth, Wheight] = size;
    // berlin-sans-fb-demi-bold
    sf::Font font("Assets/Fonts/BerlinSans.ttf"); // Throws sf::Exception if an error occurs
    // Main text (created once)
    sf::Text mainText(font);
    mainText.setString("Options");
    mainText.setPosition({ 10, 10 });
    mainText.setCharacterSize(24);
    mainText.setFillColor(sf::Color(255, 165, 0)); // orange

    // Button rectangle
    sf::Vector2f buttonSize{ 200.f, 60.f };
    sf::RectangleShape button1(buttonSize);
    button1.setFillColor(sf::Color(70, 70, 70));
    button1.setOutlineColor(sf::Color::White);
    button1.setOutlineThickness(2.f);
    button1.setPosition({ 10, 10 + buttonSize.y + margin });
    vector<float> button1Location{
        10,
        10+ buttonSize.x,
        10 + buttonSize.y + margin ,
        10 + 2*buttonSize.y+ margin,
    };

    // Button label
    sf::Text button1Label(font);
    button1Label.setCharacterSize(20);
    button1Label.setFillColor(sf::Color::White);
    button1Label.setPosition({ 10, 10 + buttonSize.y + margin });
    vector<string> messages1{
    "Hello world",
    "SubShootica!",
    "Button clicked",
    "SFML + CMake"
    };
    button1Label.setString(messages1[LabelI1]);
    // Button rectangle
    sf::RectangleShape button2(buttonSize);
    button2.setFillColor(sf::Color(70, 70, 70));
    button2.setOutlineColor(sf::Color::White);
    button2.setOutlineThickness(2.f);
    button2.setPosition({ 10, 10 + 2 * buttonSize.y + 2 * margin });
    vector<float> button2Location{
        10,
        10 + buttonSize.x,
        10 + 2*buttonSize.y + 2*margin ,
        10 + 3 * buttonSize.y + 2*margin,
    };

    // Button label
    sf::Text button2Label(font);
    button2Label.setCharacterSize(20);
    button2Label.setFillColor(sf::Color::White);
    button2Label.setPosition({ 10, 10 + 2*buttonSize.y + 2*margin });
    vector<string> messages2{
    "Hello world2",
    "SubShootica!2",
    "Button clicked2",
    "SFML + CMake2"
    };
    button2Label.setString(messages2[LabelI2]);
    // Button rectangle
    sf::RectangleShape buttonExit(buttonSize);
    buttonExit.setFillColor(sf::Color(70, 70, 70));
    buttonExit.setOutlineColor(sf::Color::White);
    buttonExit.setOutlineThickness(2.f);
    buttonExit.setPosition({ (Wwidth- buttonSize.x - margin - 10), (Wheight - buttonSize.y - margin - 10) });
    vector<float> buttonExitLocation{
        Wwidth- buttonSize.x - margin - 10,
        Wwidth - margin - 10,
		Wheight - buttonSize.y - margin - 10,
		Wheight - margin-10,
    };

    // Button label
    sf::Text buttonExitLabel(font);
    buttonExitLabel.setCharacterSize(20);
    buttonExitLabel.setFillColor(sf::Color::White);
    buttonExitLabel.setPosition({ (Wwidth - buttonSize.x - margin - 10), (Wheight - buttonSize.y - margin - 10) });
    buttonExitLabel.setString("Main Menu");

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
                cout << "mouse x: " << mouseButtonPressed->position.x << ", mouse y: " << mouseButtonPressed->position.y << "  |  " << button2Location[0] << "  -  " << button2Location[2] << endl;
                if ((mouseButtonPressed->position.x >= button1Location[0]) && (mouseButtonPressed->position.x <= button1Location[1]) && (mouseButtonPressed->position.y >= button1Location[2]) && (mouseButtonPressed->position.y <= button1Location[3])) {
                    LabelI1++;
                    if (LabelI1 >= messages1.size()) {
                        LabelI1 = 0;
                    }

                }
                if ((mouseButtonPressed->position.x >= button2Location[0]) && (mouseButtonPressed->position.x <= button2Location[1]) && (mouseButtonPressed->position.y >= button2Location[2]) && (mouseButtonPressed->position.y <= button2Location[3])) {
                    LabelI2++;
                    if (LabelI2 >= messages2.size()) {
                        LabelI2 = 0;
                    }

                }
                if ((mouseButtonPressed->position.x >= buttonExitLocation[0]) && (mouseButtonPressed->position.x <= buttonExitLocation[1]) && (mouseButtonPressed->position.y >= buttonExitLocation[2]) && (mouseButtonPressed->position.y <= buttonExitLocation[3])) {
                    *mainMenuOn = true;
                    *optionsMenuOn = false;
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
    window.draw(buttonExit);
    window.draw(buttonExitLabel);
    window.draw(button1);
    window.draw(button1Label);
    window.draw(button2);
    window.draw(button2Label);
    window.display();
}