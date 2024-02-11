#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <stdlib.h>

#include "PlayerClass.cpp"
//#include "MapClass.cpp"
#include "main.h"


// static variables initialisation
float Ray::rayMaxLength = 100.f;
int TileMap::tileSize = 32;

int TileMap::sizeX = 9, TileMap::sizeY = 10;
int TileMap::levelWalls[90] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 1, 0, 1,
    1, 0, 1, 0, 0, 1, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1,
};
int TileMap::levelFloor[90] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 1, 0, 1, 1, 1, 0,
    0, 1, 0, 1, 1, 1, 0, 1, 0,
    0, 1, 0, 1, 1, 0, 0, 1, 0,
    0, 1, 1, 1, 1, 1, 0, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 2, 1, 0, 1, 1, 1, 0,
    0, 1, 1, 2, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 500), "Go, rectangle, go!");    // A window named "window"
    //sf::RectangleShape rectangle(sf::Vector2f(50.f, 50.f));   // Rectangle
    //rectangle.setPosition(50.f, 50.f);
    //rectangle.setFillColor(sf::Color::Green);   // Set color to the shape

    //////////////////////////////////////////
    // Some game settings ////////////////////

    bool gameClosed = false;

    const int maxFPS = 60;   // Max FPS
    const int FrameTime = 1000 / maxFPS;    // Periodicity of frame changing

    //sf::Color colors[3];
    //colors[0] = sf::Color::Red; colors[1] = sf::Color::Green; colors[2] = sf::Color::Blue;
    //int colorsCounter = 0;

    Player player;
    
    TileMap map; //mapa
    map.create();

    
    //sf::Text text;
    //sf::Font font;
    //font.loadFromFile("ArialRegular.ttf");
    //text.setFont(font);
    //text.setCharacterSize(24);
    //text.setFillColor(sf::Color::Green);
    //text.setPosition(10.f, 500.f);

    //char string[12] = "";//
    //_itoa_s(sizeof(map), string, 10);
    //text.setString(string);


    // END Some game settings/////////////////
    //////////////////////////////////////////


    //////////////////////////////////////////
    // Before start cycle ////////////////////

    auto nextFrameTime = std::chrono::steady_clock::now();

    while (window.isOpen())
    {
        nextFrameTime += std::chrono::milliseconds(FrameTime);

        sf::Event event;    // Pool of every event that happened

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close(); gameClosed = true; break;

            case sf::Event::KeyPressed:     // If any key pressed
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { // Close window when "Esc" pressed
                    window.close(); gameClosed = true; break;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { // Runs debug when pressed
                    break;
                }

                //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {  // Change rectangle color
                //    rectangle.setFillColor(colors[colorsCounter]);
                //    colorsCounter = (colorsCounter + 1) % 3;
                //}

                //if (event.type == sf::Event::KeyPressed) {      // Move player
                //}

                break;

            default:
                break;
            }
        }

        player.action(event);
        player.lookAround();

        if (gameClosed) break;

        std::this_thread::sleep_until(nextFrameTime);
        window.clear(sf::Color(100, 107, 99));
        window.draw(map);
        window.draw(player);
        //window.draw(rectangle);
        //window.draw(text);
        window.display();
    }
    
    return 0;
}