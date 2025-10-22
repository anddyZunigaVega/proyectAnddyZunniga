#include "Tablero.h"
#include <iostream>
#include <cmath>

Tablero::Tablero() {
    try {
        if (!gemTex.loadFromFile("imagenes/gemmas.png")) {
            throw runtime_error("Error al cargar imagen gemmas.png");
        }
        if (!iceTex.loadFromFile("imagenes/ice_gem.png")) {
            throw runtime_error("Error al cargar ice_gem.png");
        }
        if (!bombTex.loadFromFile("imagenes/bomb_gem.png")) {
            throw runtime_error("Error al cargar bomb_gem.png");
        }
        if (!iceDamageTex.loadFromFile("imagenes/iceDamage_gem.png")) {
            throw runtime_error("Error al cargar iceDamage_gem.png");
        }
        srand(time(NULL));
        score = 0;
        moves = 20;
        win = false;
        state = IDLE;

        level = 1;
        iceRemaining = 0;
        bombsActivated = 0;

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                grid[i][j] = NULL;

        setLevel(level);
        restart();
    }
    catch (const runtime_error& e) {
        cout << "Error en Tablero::Tablero(): " << e.what() << endl;
        exit(1);
    }
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
int Tablero::getTargetScore()
{
    return targetScore;
}

bool Tablero::getStateIDLE()
{
    if (state == IDLE) {
        return true;
    }
    return false;
}
int Tablero::getLevel() {
    return level; 
}
bool Tablero::getWin()
{
    return win;
}
int Tablero::getIceRemaining() { 
    return iceRemaining;
}
int Tablero::getBombsActivated() { 
    return bombsActivated; 
}

void Tablero::setLevel(int newLevel) {
    level = newLevel;

    if (level == 1) {
        targetScore = 1250;
        targetMoves = 22;
        targetIce = 0;
        targetBombsToActivate = 0;
    }
    else if (level == 2) {
        targetScore = 1500;
        targetMoves = 25;
        targetBombsToActivate = 0;
    }
    else if (level == 3) {
        targetScore = 2200;
        targetMoves = 30;
        targetIce = 0;
        targetBombsToActivate = 5;
    }

    iceRemaining = 0;
    bombsActivated = 0;
   
    moves = targetMoves;
    if (!win) {
        score = 0;
        restart();
    }
    else{
        moves = 0;
    }
}

void Tablero::restart() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            delete grid[i][j];
            int tipo;
            do {
                tipo = rand() % 5;
            } while (createInitialBoard(i, j, tipo));
            grid[i][j] = new GemaNormal(tipo, gemTex, i, j);
        }
    }

    // nivel 2: hielo
    if (level == 2) {
        int iceRows[] = { 2,2,2,2,3,3,3,3 };
        int iceCols[] = { 1,2,5,6,1,2,5,6 };
        for (int i = 0; i < 8; i++) {
            int r = iceRows[i];
            int c = iceCols[i];
            delete grid[r][c];
            grid[r][c] = new GemaHielo(iceTex, r, c);
        }
        iceRemaining = 8;
        targetIce = 8;
    }

    // nivel 3: hielo con otras posiciones
    if (level == 3) {
        int iceRows[] = { 1,1,3,3,4,4,6,6 };
        int iceCols[] = { 3,4,1,6,1,6,3,4 };
        for (int i = 0; i < 8; i++) {
            int r = iceRows[i];
            int c = iceCols[i];
            delete grid[r][c];
            grid[r][c] = new GemaHielo(iceTex, r, c);
        }
        iceRemaining = 8;
        targetIce = 8;
    }
}

bool Tablero::createInitialBoard(int i, int j, int gemType) {
    if (j >= 2 && grid[i][j - 1] && grid[i][j - 2] &&
        grid[i][j - 1]->type == gemType && grid[i][j - 2]->type == gemType) {
        return true;
    }

    if (i >= 2 && grid[i - 1][j] && grid[i - 2][j] &&
        grid[i - 1][j]->type == gemType && grid[i - 2][j]->type == gemType) {
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
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - 2; j++) {
            if (grid[i][j] && grid[i][j + 1] && grid[i][j + 2]) {
                if (grid[i][j]->type != -1 && grid[i][j]->type < 5 &&
                    grid[i][j]->type == grid[i][j + 1]->type &&
                    grid[i][j]->type == grid[i][j + 2]->type) {
                    return true;
                }
            }
        }
    }

    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N - 2; i++) {
            if (grid[i][j] && grid[i + 1][j] && grid[i + 2][j]) {
                if (grid[i][j]->type != -1 && grid[i][j]->type < 5 &&
                    grid[i][j]->type == grid[i + 1][j]->type &&
                    grid[i][j]->type == grid[i + 2][j]->type) {
                    return true;
                }
            }
        }
    }

    return false;
}


void Tablero::deleteMatch() {
    bool markBoard[N][N] = { false };
    bool bombCreate[N][N] = { false };   // marca donde crear una bomba
    int matchLength[N][N] = { 0 };       // guarda tamaño del match

    // Buscar matches horizontales
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - 2; j++) {
            if (!grid[i][j] || grid[i][j]->type < 0) {
                continue;
            }
            int t = grid[i][j]->type;
            int k = j + 1;

            while (k < N && grid[i][k] && grid[i][k]->type == t) {
                k++;
            }

            int len = k - j;
            if (len >= 3 && t < 5) {
                for (int x = j; x < k; x++) {
                    markBoard[i][x] = true;
                    matchLength[i][x] = len;
                }
                // Si es >=4 y estamos en nivel 3, marcar la PRIMERA posición (izquierda)
                if (len >= 4 && level == 3) {
                    bombCreate[i][j] = true;
                }
            }
            j = k - 1;
        }
    }

    //Buscar matches verticales
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N - 2; i++) {
            if (!grid[i][j] || grid[i][j]->type < 0) {
                continue;
            }
            int t = grid[i][j]->type;
            int k = i + 1;

            while (k < N && grid[k][j] && grid[k][j]->type == t) {
                k++;
            }

            int len = k - i;
            if (len >= 3 && t < 5) {
                for (int x = i; x < k; x++) {
                    markBoard[x][j] = true;
                    matchLength[x][j] = len;
                }
                // Si es >=4 y estamos en nivel 3, marcar la PRIMERA posición (arriba)
                if (len >= 4 && level == 3) {
                    bombCreate[i][j] = true;
                }
            }
            i = k - 1;
        }
    }

    //Daño a hielos adyacentes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (markBoard[i][j]) {
                static const int di[4] = { -1, 1, 0, 0 };
                static const int dj[4] = { 0, 0, -1, 1 };
                for (int d = 0; d < 4; d++) {
                    int ni = i + di[d];
                    int nj = j + dj[d];
                    if (ni >= 0 && ni < N && nj >= 0 && nj < N && grid[ni][nj]) {
                        if (grid[ni][nj]->type == 5) {
                            GemaHielo* ice = (GemaHielo*)grid[ni][nj]; // cast directo: tratar gemanormal como gemahileo
                            ice->takeHit();
                         
                            if (ice->hits == 1) {
                                ice->setDamagedTexture(iceDamageTex);
                            }
                            else if (ice->hits <= 0) {
                                delete grid[ni][nj];
                                grid[ni][nj] = nullptr;
                                iceRemaining--;
                                score += 10;
                            }
                        }
                    }
                }
            }
        }
    }

    // Eliminar gemas marcadas y crear bombas solo donde bombCreate==true
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (!markBoard[i][j] || !grid[i][j]) {
                continue;
            }

            int type = grid[i][j]->type;

            // No eliminar hielos ni bombas 
            if (type == 5 || type == 6) {
                continue;
            }

            // Crear bomba SOLO si esta celda fue marcada
            if (bombCreate[i][j] && matchLength[i][j] >= 4 && level == 3) {
                delete grid[i][j];
                grid[i][j] = new GemaBomba(bombTex, i, j);
            }
            else {
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
    for (int j = 0; j < N; j++) {
        int writeRow = N - 1;
        for (int i = N - 1; i >= 0; i--) {
            if (grid[i][j] && grid[i][j]->type != -1) {
                if (i != writeRow) {
                    std::swap(grid[writeRow][j], grid[i][j]);
                    grid[writeRow][j]->targetPos = Vector2f(j * 130, writeRow * 130);
                }
                writeRow--;
            }
        }
        for (int i = writeRow; i >= 0; i--) {
            delete grid[i][j];
            grid[i][j] = new GemaNormal(rand() % 5, gemTex, i, j);
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

    if (!grid[row1][col1] || !grid[row2][col2]) {
        return false;
    }

    int t1 = grid[row1][col1]->type;
    int t2 = grid[row2][col2]->type;

    // Nivel 2 y 3: hielo no swap
    if ((level == 2 || level == 3) && (t1 == 5 || t2 == 5)) {
        return false;
    }
    // Bomba en nivel 3: explota quitando fila horizontal
    if (level == 3 && (t1 == 6 || t2 == 6)) {
        int bombRow;
        if (t1 == 6) {
            bombRow = row1;
        }
        else {
            bombRow = row2;
        }

        for (int c = 0; c < N; c++) {
            if (grid[bombRow][c]) {
                if (grid[bombRow][c]->type == 5) {
                    iceRemaining--;
                }
                delete grid[bombRow][c];
                grid[bombRow][c] = nullptr;
                score += 10;
            }
        }
        bombsActivated++;
        moves--;
        applyGravityAndGenerate();
        state = FALLING;
        return true;
    }

    // Swap normal
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
                if (grid[firstCell.y][firstCell.x]) grid[firstCell.y][firstCell.x]->targetPos = Vector2f(firstCell.x * 130, firstCell.y * 130);
                if (grid[secondCell.y][secondCell.x]) grid[secondCell.y][secondCell.x]->targetPos = Vector2f(secondCell.x * 130, secondCell.y * 130);
                state = REVERSING;
            }
        }
        break;

    case REVERSING:
        if (!updateAnimation(time, SWAP_SPEED)) state = IDLE;
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
            else state = IDLE;
        }
        break;

    case IDLE:
        break;
    }

    if (state == IDLE && level == 3 &&
        score >= targetScore && iceRemaining <= 0 && bombsActivated >= targetBombsToActivate) {
        win = true;
    }
}

bool Tablero::objectivesMet() {
    if (level == 1) {
        return score >= targetScore;
    }
    if (level == 2) {
        return score >= targetScore && iceRemaining <= 0;
    }

    if (level == 3) {
        return score >= targetScore && iceRemaining <= 0 && bombsActivated >= targetBombsToActivate;
    }
    return false;
}