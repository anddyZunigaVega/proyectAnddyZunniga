#include <iostream>
#include "Juego.h"
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;


bool isMouseOver(const RectangleShape& button, const RenderWindow& window) {
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
    return button.getGlobalBounds().contains(mousePos);
}

//menu principal
int main() {
    RenderWindow window(VideoMode(1920, 1080), "Menu Principal Race Crash");
    
    Texture backgroundTex;
    if (!backgroundTex.loadFromFile("imagenes/motos_fondo_512.png")){
        cout << "Error imagen menu principal." << std::endl;
        return -1;
    }

    Sprite background(backgroundTex);

    RectangleShape playButton(Vector2f(340, 120));
    playButton.setPosition(800, 610);
    playButton.setFillColor(Color::Transparent);

    RectangleShape exitButton(Vector2f(340, 120));
    exitButton.setPosition(800, 790);
    exitButton.setFillColor(Color::Transparent);


    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button==Mouse::Left) {
                
              if (isMouseOver(playButton, window)) {
                        Juego game;
                        game.run();
              }
              if (isMouseOver(exitButton, window)) {
                        window.close();
              }
                
            }
        }

        window.clear();
        window.draw(background);   
        window.draw(playButton);
    
        window.draw(exitButton);
        window.display();
    }

    return 0;
}