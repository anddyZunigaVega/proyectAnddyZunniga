#include <iostream>
#include "Juego.h"
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

bool isMouseOver(const RectangleShape& button, const RenderWindow& window) {
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
    return button.getGlobalBounds().contains(mousePos);
}

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Menu Principal Race Crash");
    
    Texture backgroundTex;
    if (!backgroundTex.loadFromFile("imagenes/motos_fondo_512.png")){
        cout << "Error imagen menu principal." << std::endl;
        return -1;
    }

    Sprite background(backgroundTex);

    RectangleShape botonJugar(Vector2f(340, 120));
    botonJugar.setPosition(800, 610);
    botonJugar.setFillColor(Color::Transparent);

    RectangleShape botonSalir(Vector2f(340, 120));
    botonSalir.setPosition(800, 790);
    botonSalir.setFillColor(Color::Transparent);


    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button==Mouse::Left) {
                
              if (isMouseOver(botonJugar, window)) {
                        Juego juego;
                        juego.run();
              }
              if (isMouseOver(botonSalir, window)) {
                        window.close();
              }
                
            }
        }

        window.clear();
        window.draw(background);   
        window.draw(botonJugar);
    
        window.draw(botonSalir);
        window.display();
    }

    return 0;
}