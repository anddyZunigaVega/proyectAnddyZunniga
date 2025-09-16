#include "Juego.h"
#include <iostream>

Juego::Juego() : window(VideoMode(1920,1080), "Rally Crash"), selected(false) {

    if (!font.loadFromFile("letras/arial.ttf")) {
        cout << "Error cargando letra arial.ttf\n";
    }

    
    if (!backGroundTex1.loadFromFile("imagenes/fondo.png")) {
        cout<< "Error al cargar imagen de fondo normal." << endl;
    }
    if (!backGroundTex2.loadFromFile("imagenes/fondo2.png")) {
        cout << "Error al cargar imagen de fondo final." << endl;
    }

    
    backGround1.setTexture(backGroundTex1);
    backGround2.setTexture(backGroundTex2);
   
    scoreText.setFont(font);
    scoreText.setCharacterSize(35);
    scoreText.setFillColor(Color::Cyan);
    scoreText.setPosition(1250, 800); 


    finalScore.setFont(font);
    finalScore.setCharacterSize(40);
    finalScore.setFillColor(Color::Cyan);
    finalScore.setPosition(1350, 650);


    
    exitButton = RectangleShape(Vector2f(125, 75));
    exitButton.setPosition(1725, 925);
    exitButton.setFillColor(Color::Transparent);
}

void Juego::run() {
    while (window.isOpen()) {
        float time = clock.restart().asSeconds();
        processEvents();
        tablero.update(time); 
        render();
    }
}

void Juego::processEvents() {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
        }
        else if (event.type == Event::MouseButtonPressed) {
           
            Vector2f pos = window.mapPixelToCoords(Mouse::getPosition(window)); 
            int row = pos.y / 130;
            int col = pos.x / 130;

            if (!selected) {
                firstSelect = { col, row };
                selected = true;
            }
            else {
                tablero.tryMove(firstSelect.y, firstSelect.x, row, col);
                selected = false;
            }

            Vector2f mouseCords = window.mapPixelToCoords(sf::Mouse::getPosition(window)); 
            
            if (exitButton.getGlobalBounds().contains(pos)) {
                window.close();
            }

        }
    }
}

void Juego::render() {

    window.clear(Color::White);

    window.draw(backGround1);
    tablero.draw(window);

    
    scoreText.setString("                                  " + to_string(tablero.getMoves()) + "\n\n   " + to_string(tablero.getScore()));

    if (tablero.getMoves() != 0) {
        window.draw(backGround1);
        tablero.draw(window);
        window.draw(scoreText);
    }
    
    if (tablero.getMoves() == 0) {
        window.draw(backGround2);
        tablero.draw(window);

        finalScore.setString(to_string(tablero.getScore()));
        window.draw(finalScore);
    }

    window.draw(exitButton);
    window.display();
}
