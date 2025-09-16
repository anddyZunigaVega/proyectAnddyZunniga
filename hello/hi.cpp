//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <vector>
//#include <ctime>
//#include <cmath>
//#include <cstring>
//
//const int TILE_SIZE = 64;
//const int ROWS = 8;
//const int COLS = 8;
//const int GEM_TYPES = 5;
//
//const float SWAP_SPEED = 600.f;
//const float FALL_SPEED = 900.f;
//const sf::Time MATCH_PAUSE = sf::milliseconds(300);
//
//enum GameState { IDLE, SWAPPING, REVERSING, MATCHING_PAUSE, FALLING };
//
//int grid[ROWS][COLS];
//sf::Texture texture;
//sf::Sprite sprites[ROWS][COLS];
//sf::Vector2f targetPos[ROWS][COLS];
//bool toClear[ROWS][COLS];
//
//GameState state = IDLE;
//sf::Vector2i firstCell(-1, -1), secondCell(-1, -1);
//sf::Clock frameClock, pauseClock;
//
//int movesLeft = 20;
//int score = 0;
//
//inline sf::Vector2f slotPos(int r, int c) { return sf::Vector2f(c * TILE_SIZE, r * TILE_SIZE); }
//
//bool hasAnyMatch() {
//    for (int i = 0; i < ROWS; ++i) {
//        int j = 0;
//        while (j < COLS) {
//            int g = grid[i][j]; if (g == -1) { ++j; continue; }
//            int k = j + 1; while (k < COLS && grid[i][k] == g) ++k;
//            if (k - j >= 3) return true; j = k;
//        }
//    }
//    for (int j = 0; j < COLS; ++j) {
//        int i = 0;
//        while (i < ROWS) {
//            int g = grid[i][j]; if (g == -1) { ++i; continue; }
//            int k = i + 1; while (k < ROWS && grid[k][j] == g) ++k;
//            if (k - i >= 3) return true; i = k;
//        }
//    }
//    return false;
//}
//
//int findMatchesAndMark() {
//    std::memset(toClear, 0, sizeof(toClear));
//    int count = 0;
//    // filas
//    for (int i = 0; i < ROWS; ++i) {
//        int j = 0;
//        while (j < COLS) {
//            int g = grid[i][j]; if (g == -1) { ++j; continue; }
//            int k = j + 1; while (k < COLS && grid[i][k] == g) ++k;
//            int run = k - j;
//            if (run >= 3) for (int c = j; c < k; ++c) { if (!toClear[i][c]) { toClear[i][c] = true; ++count; } }
//            j = k;
//        }
//    }
//    // columnas
//    for (int j = 0; j < COLS; ++j) {
//        int i = 0;
//        while (i < ROWS) {
//            int g = grid[i][j]; if (g == -1) { ++i; continue; }
//            int k = i + 1; while (k < ROWS && grid[k][j] == g) ++k;
//            int run = k - i;
//            if (run >= 3) for (int r = i; r < k; ++r) { if (!toClear[r][j]) { toClear[r][j] = true; ++count; } }
//            i = k;
//        }
//    }
//    return count;
//}
//
//void clearMarked() {
//    for (int i = 0; i < ROWS; ++i)
//        for (int j = 0; j < COLS; ++j)
//            if (toClear[i][j]) { grid[i][j] = -1; sprites[i][j].setColor(sf::Color::White); }
//}
//
//void initBoardNoMatches() {
//    std::srand(static_cast<unsigned>(std::time(nullptr)));
//    do {
//        for (int i = 0; i < ROWS; ++i)
//            for (int j = 0; j < COLS; ++j)
//                grid[i][j] = std::rand() % GEM_TYPES;
//    } while (hasAnyMatch());
//
//    for (int i = 0; i < ROWS; ++i)
//        for (int j = 0; j < COLS; ++j) {
//            sprites[i][j].setTexture(texture);
//            sprites[i][j].setTextureRect(sf::IntRect(grid[i][j] * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
//            sprites[i][j].setPosition(slotPos(i, j));
//            targetPos[i][j] = slotPos(i, j);
//            sprites[i][j].setColor(sf::Color::White);
//        }
//}
//
//bool updateAnimations(float dt, float speed) {
//    bool moving = false;
//    for (int i = 0; i < ROWS; ++i)
//        for (int j = 0; j < COLS; ++j) {
//            sf::Vector2f pos = sprites[i][j].getPosition();
//            sf::Vector2f tgt = targetPos[i][j];
//            sf::Vector2f diff = tgt - pos;
//            float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
//            if (dist > 0.5f) {
//                sf::Vector2f dir = diff / dist;
//                float step = speed * dt;
//                if (step >= dist) sprites[i][j].setPosition(tgt);
//                else sprites[i][j].move(dir * step);
//                moving = true;
//            }
//            else sprites[i][j].setPosition(tgt);
//        }
//    return moving;
//}
//
//void applyGravityAndSpawn() {
//    for (int j = 0; j < COLS; ++j) {
//        int writeRow = ROWS - 1;
//        for (int i = ROWS - 1; i >= 0; --i) {
//            if (grid[i][j] != -1) {
//                if (i != writeRow) { std::swap(grid[writeRow][j], grid[i][j]); std::swap(sprites[writeRow][j], sprites[i][j]); }
//                --writeRow;
//            }
//        }
//        for (int i = writeRow; i >= 0; --i) {
//            grid[i][j] = std::rand() % GEM_TYPES;
//            sprites[i][j].setTextureRect(sf::IntRect(grid[i][j] * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
//            sprites[i][j].setPosition(sf::Vector2f(j * TILE_SIZE, -TILE_SIZE * (writeRow - i + 1)));
//            targetPos[i][j] = slotPos(i, j);
//            sprites[i][j].setColor(sf::Color::White);
//        }
//    }
//}
//
//void drawBoard(sf::RenderWindow& window, bool tintMarked) {
//    for (int i = 0; i < ROWS; ++i)
//        for (int j = 0; j < COLS; ++j) {
//            if (tintMarked && toClear[i][j]) {
//                sf::Sprite s = sprites[i][j]; s.setColor(sf::Color(255, 255, 255, 140)); window.draw(s);
//            }
//            else window.draw(sprites[i][j]);
//        }
//}
//
//int main() {
//    const int UI_HEIGHT = 50;
//    sf::RenderWindow window(sf::VideoMode(COLS * TILE_SIZE, ROWS * TILE_SIZE + UI_HEIGHT), "Gema Crash");
//    window.setFramerateLimit(60);
//
//    if (!texture.loadFromFile("imagenes/gemmas.png")) { std::cerr << "No se pudo cargar imagen\n"; return 1; }
//
//    initBoardNoMatches();
//
//    sf::Font font;
//    if (!font.loadFromFile("letras/arial.ttf")) { std::cerr << "No se pudo cargar fuente\n"; return 1; }
//    sf::Text moveText, scoreText;
//    moveText.setFont(font); moveText.setCharacterSize(24); moveText.setFillColor(sf::Color::Black);
//    scoreText.setFont(font); scoreText.setCharacterSize(24); scoreText.setFillColor(sf::Color::Yellow);
//
//    bool firstSelected = false;
//
//    while (window.isOpen()) {
//        sf::Event e;
//        while (window.pollEvent(e)) {
//            if (e.type == sf::Event::Closed) window.close();
//            if (state == IDLE && e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
//                sf::Vector2i mouse = sf::Mouse::getPosition(window);
//                int c = mouse.x / TILE_SIZE, r = mouse.y / TILE_SIZE;
//                if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
//                    if (!firstSelected) { firstCell = { c,r }; firstSelected = true; }
//                    else {
//                        secondCell = { c,r };
//                        if ((std::abs(firstCell.x - secondCell.x) + std::abs(firstCell.y - secondCell.y)) == 1) {
//                            std::swap(grid[firstCell.y][firstCell.x], grid[secondCell.y][secondCell.x]);
//                            std::swap(sprites[firstCell.y][firstCell.x], sprites[secondCell.y][secondCell.x]);
//                            targetPos[firstCell.y][firstCell.x] = slotPos(firstCell.y, firstCell.x);
//                            targetPos[secondCell.y][secondCell.x] = slotPos(secondCell.y, secondCell.x);
//                            state = SWAPPING;
//                        }
//                        firstSelected = false;
//                    }
//                }
//            }
//        }
//
//        float dt = frameClock.restart().asSeconds();
//
//        switch (state) {
//        case SWAPPING: {
//            bool moving = updateAnimations(dt, SWAP_SPEED);
//            if (!moving) {
//                int marked = findMatchesAndMark();
//                if (marked > 0) {
//                    movesLeft--; score += 10 * marked;
//                    pauseClock.restart(); state = MATCHING_PAUSE;
//                }
//                else {
//                    std::swap(grid[firstCell.y][firstCell.x], grid[secondCell.y][secondCell.x]);
//                    std::swap(sprites[firstCell.y][firstCell.x], sprites[secondCell.y][secondCell.x]);
//                    targetPos[firstCell.y][firstCell.x] = slotPos(firstCell.y, firstCell.x);
//                    targetPos[secondCell.y][secondCell.x] = slotPos(secondCell.y, secondCell.x);
//                    state = REVERSING;
//                }
//            }
//        } break;
//        case REVERSING: if (!updateAnimations(dt, SWAP_SPEED)) state = IDLE; break;
//        case MATCHING_PAUSE: if (pauseClock.getElapsedTime() >= MATCH_PAUSE) { clearMarked(); applyGravityAndSpawn(); state = FALLING; } break;
//        case FALLING: if (!updateAnimations(dt, FALL_SPEED)) { int marked = findMatchesAndMark(); if (marked > 0) { movesLeft--; score += 10 * marked; pauseClock.restart(); state = MATCHING_PAUSE; } else state = IDLE; } break;
//        case IDLE: break;
//        }
//
//        window.clear(sf::Color::White);
//        drawBoard(window, state == MATCHING_PAUSE);
//        moveText.setString("Movimientos: " + std::to_string(movesLeft));
//        scoreText.setString("Puntos: " + std::to_string(score));
//        moveText.setPosition(5, ROWS * TILE_SIZE + 5);
//        scoreText.setPosition(300, ROWS * TILE_SIZE + 5);
//        window.draw(moveText); window.draw(scoreText);
//        window.display();
//    }
//
//    return 0;
//}