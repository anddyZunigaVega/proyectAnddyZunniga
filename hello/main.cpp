#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include "Juego.h"
#include "Lista.h"
#include "Ranking.h"

using namespace sf;
using namespace std;


bool isMouseOver(const RectangleShape& button, const RenderWindow& window) {
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
    return button.getGlobalBounds().contains(mousePos);
}

// Ventana escribir nombre del jugador
string pedirNombre() {
    RenderWindow nameWindow(VideoMode(600, 300), "Ingresa tu nombre");
    Font font;
    if (!font.loadFromFile("assets/letras/arial.ttf")) {
        cout << "Error cargando fuente arial.ttf" << endl;
        return "";
    }

    Text textoInstruccion("Escribe tu nombre y presiona Enter:", font, 28);
    textoInstruccion.setFillColor(Color::White);
    textoInstruccion.setPosition(40, 50);

    Text textoNombre("", font, 32);
    textoNombre.setFillColor(Color::Yellow);
    textoNombre.setPosition(80, 140);

    string input = "";

    while (nameWindow.isOpen()) {
        Event event;
        while (nameWindow.pollEvent(event)) {
            if (event.type == Event::Closed) {
                nameWindow.close();
                return "";
            }

            if (event.type == Event::TextEntered) {
                if (event.text.unicode == '\b') { // borrar
                    if (!input.empty()) input.pop_back();
                }
                else if (event.text.unicode == '\r') { // enter guardar nombre
                    if (!input.empty()) {
                        nameWindow.close();
                        return input;
                    }
                }
                else if (event.text.unicode < 128 && isprint(event.text.unicode)) {
                    if (input.size() < 15)
                        input += static_cast<char>(event.text.unicode);
                }
                textoNombre.setString(input);
            }
        }

        nameWindow.clear(Color(20, 20, 20));
        nameWindow.draw(textoInstruccion);
        nameWindow.draw(textoNombre);
        nameWindow.display();
    }
    return "";
}


int main() {
    RenderWindow window(VideoMode(1920, 1080), "Menu Principal Race Crash");

    Texture backgroundTex;
    if (!backgroundTex.loadFromFile("assets/imagenes/motos_fondo_512.png")) {
        cout << "Error imagen menu principal." << endl;
        return -1;
    }
    Sprite background(backgroundTex);

    // Botones
    RectangleShape playButton(Vector2f(340, 120));
    playButton.setPosition(800, 610);
    playButton.setFillColor(Color::Transparent);

    RectangleShape exitButton(Vector2f(340, 120));
    exitButton.setPosition(800, 790);
    exitButton.setFillColor(Color::Transparent);

    // LEER RANKING
    Lista lista;
    ifstream file("ranking.txt");
    if (file.is_open()) {
        string name;
        int score, moves;
        while (file >> name >> score >> moves) {
            Ranking* r = new Ranking(name, score, moves);
            lista.agregarOrdenado(r);
        }
        file.close();
    }
    else {
        cout << "No se encontro ranking.txt. Se creara al guardar." << endl;
    }

    Font font;
    if (!font.loadFromFile("assets/letras/arial.ttf")) {
        cout << "Error cargando fuente arial.ttf\n";
    }

    Text rankingText;
    rankingText.setFont(font);
    rankingText.setCharacterSize(28);
    rankingText.setFillColor(Color::Green);
    rankingText.setPosition(20, 500);
    rankingText.setString("TOP 10 SCORE por Movimientos:\n\n" + lista.getPrimerosDiez());

    string nombreJugador = ""; // nombre temporal

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (isMouseOver(playButton, window)) {
                    nombreJugador = pedirNombre(); // Ventana para ingresar nombre
                    if (!nombreJugador.empty()) {
                        // Guardar el nombre en un archivo temporal para Tablero
                        ofstream temp("nombre_actual.txt");
                        temp << nombreJugador;
                        temp.close();

                        Juego game;
                        game.run();
                    }
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
        window.draw(rankingText);
        window.display();
    }

    return 0;
}