#include "Juego.h"
#include <iostream>
using namespace std;

Juego::Juego() : window(VideoMode(1920, 1080), "Rally Crash"), selected(false) {
    try {
        if (!font.loadFromFile("letras/arial.ttf")) {
            throw runtime_error("Error cargando letra arial.ttf");
        }

        if (!backGroundTex1.loadFromFile("imagenes/fondo.png")) {
            throw runtime_error("Error al cargar imagen de fondo normal.");
        }
        if (!backGroundTex2.loadFromFile("imagenes/fondoPerdida.png")) {
            throw runtime_error("Error al cargar imagen de fondo final.");
        }
        if (!backGroundTexLevel2.loadFromFile("imagenes/fondo2.png")) {
            throw runtime_error("No se encontro fondoNivel2.png");
        }
        if (!backGroundTexLevel3.loadFromFile("imagenes/fondo3.png")) {
            throw runtime_error("No se encontro fondoNivel3.png");
        }
        if (!backGroundTexFinal.loadFromFile("imagenes/fondoFinall.png")) {
            throw runtime_error("No se encontro fondoFinall.png");
        }
        if (!backGroundTexWait.loadFromFile("imagenes/fondoEspera.png")) {
            throw runtime_error("No se encontro fondoEspera.png");
        }

        backGround1.setTexture(backGroundTex1);
        backGround2.setTexture(backGroundTex2);
        backGroundFinal.setTexture(backGroundTexFinal);
        backGroundLevel2.setTexture(backGroundTexLevel2);
        backGroundLevel3.setTexture(backGroundTexLevel3);
        backGroundWait.setTexture(backGroundTexWait);

        scoreText.setFont(font);
        scoreText.setCharacterSize(37);
        scoreText.setFillColor(Color::Cyan);
        scoreText.setPosition(1250, 785);

        finalScore.setFont(font);
        finalScore.setCharacterSize(40);
        finalScore.setFillColor(Color::Cyan);
        finalScore.setPosition(1350, 650);

        progressText.setFont(font);
        progressText.setCharacterSize(30);
        progressText.setFillColor(Color::White);
        progressText.setPosition(1450, 294);

        progressBar.setPosition(1288, 297);
        progressBar.setFillColor(Color::Green);

        exitButton = RectangleShape(Vector2f(125, 75));
        exitButton.setPosition(1725, 925);
        exitButton.setFillColor(Color::Transparent);
        
        progress = 0.0;
        currentLevel = tablero.getLevel();
        waitingForEnter = false;
    }
    catch (const exception& e) {
        cerr << "Excepcion en Juego() constructor: " << e.what() << endl;
        window.close();
    }
}

void Juego::run() {
    while (window.isOpen()) {
        float time = clock.restart().asSeconds();
        processEvents();

        // solo actualiza si no está esperando ENTER
        if (!waitingForEnter && !tablero.getWin()) {
            tablero.update(time);
        }

        // Me aseguro que el tablero esté estable antes de pasar de nivel, state == IDLE 
        if (!waitingForEnter && tablero.objectivesMet() && tablero.getStateIDLE() && !tablero.getWin()) {
            waitingForEnter = true;
        }

        render();
    }
}

void Juego::processEvents() {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
        }

        Vector2f pos = window.mapPixelToCoords(Mouse::getPosition(window));
        if (event.type == Event::MouseButtonPressed) {
            if (exitButton.getGlobalBounds().contains(pos)) {
                window.close();
                return;
            }
        }

        if (waitingForEnter) {//pantalla espera de siguiente nivel
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
              
                int lvl = tablero.getLevel();
                if (lvl == 1) tablero.setLevel(2);
                else if (lvl == 2) tablero.setLevel(3);
                else if (lvl == 3) tablero.setLevel(0);

                waitingForEnter = false; 
            }
            continue; // mientras está esperando no se procesa nada más
        }
        else if (event.type == Event::MouseButtonPressed) {
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
        }
    }
}

void Juego::render() {
    window.clear(Color::White);

    
    if (waitingForEnter) {
        window.draw(backGroundWait);
        tablero.draw(window);

        finalScore.setString(to_string(tablero.getScore()));
        window.draw(finalScore);
        window.draw(exitButton);
        window.display();
        return;
    }

    // Fondo por nivel
    int lvl = tablero.getLevel();
    if (lvl == 1) {
        window.draw(backGround1); 
    }
    else if (lvl == 2) { 
        window.draw(backGroundLevel2);
    }
    else if (lvl == 3) { 
        window.draw(backGroundLevel3); 
    }

    // Textos
    string s = "                                 " + to_string(tablero.getMoves()) + "\n\n  " + to_string(tablero.getScore()) + "/" + to_string(tablero.getTargetScore());



    if (tablero.getLevel() == 2) {
        s += "\n\n              " + to_string(tablero.getIceRemaining());
    }
    if (tablero.getLevel() == 3) {
        s += "\n             " + to_string(tablero.getIceRemaining());
        s += "\n   " + to_string(tablero.getBombsActivated()) + "/5";
    }
    scoreText.setString(s);
    window.draw(scoreText);

    // Barra de progreso
    progress = 0.0;
    if (tablero.getTargetScore() > 0) {
        progress = min(1.f, (float)tablero.getScore() / (float)tablero.getTargetScore());
        progressBar.setSize(Vector2f(361.f * progress, 30.f));

        int porcent = progress * 100;
        string text = to_string(porcent) + "%";
        window.draw(progressBar);
        progressText.setString(text);
        window.draw(progressText);
    }


    // Pantallas de final o pérdida
    if (tablero.getMoves() == 0 && tablero.getStateIDLE()) {
        window.draw(backGround2);
        finalScore.setString(to_string(tablero.getScore())+"/"+ to_string(tablero.getTargetScore()));
        window.draw(finalScore);
    }
    if (tablero.getWin()) {
        window.draw(backGroundFinal);
        finalScore.setString(to_string(tablero.getScore()));
        window.draw(finalScore);
    }

    window.draw(exitButton);
    tablero.draw(window);
    window.display();
}