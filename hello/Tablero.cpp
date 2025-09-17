#include "Tablero.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

Tablero::Tablero() {
    srand(time(NULL));
    score = 0;
    moves = 20;
    state = IDLE;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = NULL;
        }
    }

    if (!gemTex.loadFromFile("imagenes/gemmas.png")) {
        cout << "Error cargar imagen gemmas" << std::endl;
    }

    restart();
}

Tablero::~Tablero() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            delete grid[i][j];
}

int Tablero::getMoves() {
    return moves;
}

int Tablero::getScore() {
    return score;
}

void Tablero::restart() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            delete grid[i][j];
            int tipo;
            do {
                tipo = rand() % 5;
            } while (createInitialBoard(i, j, tipo));
            grid[i][j] = new Gema(tipo, gemTex, i, j);
        }
    }
}

bool Tablero::createInitialBoard(int i, int j, int gemType) {
    if (j >= 2 && grid[i][j - 1] && grid[i][j - 2] && grid[i][j - 1]->type == gemType && grid[i][j - 2]->type == gemType) {
        return true;
    }

    if (i >= 2 && grid[i - 1][j] && grid[i - 2][j] && grid[i - 1][j]->type == gemType && grid[i - 2][j]->type == gemType) {
        return true;
    }

    return false;
}

void Tablero::draw(RenderWindow& window) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j]) {
              window.draw(grid[i][j]->sprite);
            }
        }
    }
}

bool Tablero::anyMatch() {
    // Horizontal
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N - 2; j++) {
            int t = grid[i][j]->type;
            if (t == -1) continue;
            if (grid[i][j + 1]->type == t && grid[i][j + 2]->type == t)
                return true;
        }

    // Vertical
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N - 2; i++) {
            int t = grid[i][j]->type;
            if (t == -1) continue;
            if (grid[i + 1][j]->type == t && grid[i + 2][j]->type == t)
                return true;
        }
    }

    return false;
}

void Tablero::deleteMatch() {
    bool markBoard[N][N] = { false };

    // Revisar Horizontal
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N - 2; j++) {
            int gemType = grid[i][j]->type;
            if (gemType == -1) continue;
            if (grid[i][j + 1]->type == gemType && grid[i][j + 2]->type == gemType) {
                int gemCounter = j;
                while (gemCounter < N && grid[i][gemCounter]->type == gemType) {
                    markBoard[i][gemCounter] = true;
                    gemCounter++;
                }
            }
        }

    // Revisar Vertical
    for (int j = 0; j < N; j++)
        for (int i = 0; i < N - 2; i++) {
            int gemType = grid[i][j]->type;

            if (gemType == -1) {
                continue;
            }
            if (grid[i + 1][j]->type == gemType && grid[i + 2][j]->type == gemType) {
                int gemCounter = i;
                while (gemCounter < N && grid[gemCounter][j]->type == gemType) {
                    markBoard[gemCounter][j] = true;
                    gemCounter++;
                }
            }
        }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (markBoard[i][j]) {
                grid[i][j]->setType(-1, gemTex);
                score += 10;
            }
        }
    }
}


bool Tablero::updateAnimation(float time, float speed) {
    bool moving = false;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (!grid[i][j]) {
                continue;
            }
            Vector2f pos = grid[i][j]->sprite.getPosition();
            Vector2f tgt = grid[i][j]->targetPos;
            Vector2f diff = tgt - pos;

            float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (dist > 0.5f) {
                Vector2f dir = diff / dist;
                float step = speed * time;
                if (step >= dist) {
                    grid[i][j]->sprite.setPosition(tgt);
                }
                else {
                    grid[i][j]->sprite.move(dir * step);
                    moving = true;
                }
            }
        }
    }
    return moving;
}

void Tablero::applyGravityAndGenerate() {
    for (int j = 0; j < N; ++j) {
        int writeRow = N - 1;
        for (int i = N - 1; i >= 0; --i) {
            if (grid[i][j]->type != -1) {
                if (i != writeRow) {
                    swap(grid[writeRow][j], grid[i][j]);
                    grid[writeRow][j]->targetPos = Vector2f(j * 130, writeRow * 130);
                }
                writeRow--;
            }
        }
        for (int i = writeRow; i >= 0; --i) {
            grid[i][j]->setType(rand() % 5, gemTex);
            grid[i][j]->sprite.setPosition(j * 130, -130 * (writeRow - i + 1));
            grid[i][j]->targetPos = Vector2f(j * 130, i * 130);
        }
    }
}


bool Tablero::tryMove(int row1, int col1, int row2, int col2) {
    if (moves <= 0 || state != IDLE) {
        return false;
    }

    if (abs(row1 - row2) + abs(col1 - col2) != 1) {
        return false;
    }

    firstCell = { col1, row1 };
    secondCell = { col2, row2 };

    swap(grid[row1][col1], grid[row2][col2]);
    grid[row1][col1]->targetPos = Vector2f(col1 * 130, row1 * 130);
    grid[row2][col2]->targetPos = Vector2f(col2 * 130, row2 * 130);

    state = SWAPPING;
    return true;
}

void Tablero::update(float time) {
    switch (state) {
    case SWAPPING:
        if (!updateAnimation(time, SWAP_SPEED)) {

            
            if (anyMatch()) {
                deleteMatch();
                pauseClock.restart();
                moves--;
                state = MATCHING_PAUSE;
            }
            else {
                swap(grid[firstCell.y][firstCell.x], grid[secondCell.y][secondCell.x]);
                grid[firstCell.y][firstCell.x]->targetPos = Vector2f(firstCell.x * 130, firstCell.y * 130);
                grid[secondCell.y][secondCell.x]->targetPos = Vector2f(secondCell.x * 130, secondCell.y * 130);
                state = REVERSING;
            }
        }
        break;
    case REVERSING:
        if (!updateAnimation(time, SWAP_SPEED)) {
            state = IDLE;
        }
        break;
    case MATCHING_PAUSE:
        if (pauseClock.getElapsedTime() >= MATCH_PAUSE) {
            applyGravityAndGenerate();
            state = FALLING;
        }
        break;
    case FALLING:
        if (!updateAnimation(time, FALL_SPEED)) {
            if (anyMatch()) {
                deleteMatch();
                
                pauseClock.restart();
                state = MATCHING_PAUSE;
            }
            else {
                state = IDLE;
                
            }
            
        }
        break;
    case IDLE:
        break;
    }
}

