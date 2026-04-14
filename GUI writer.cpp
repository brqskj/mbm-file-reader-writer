/*
COMMANDS:
    - arrow keys to resize
    - left click to place wall
    - right click to delete wall
    - enter to save
*/
/*
HOW DOES THIS WORK?
    - reads input to edit the map
    - updates the map
    - draws it on screen
    - saves it trough write_mbm.h
*/

#include "raylib.h"
#include "write_mbm.hpp"


Vector2 cellsize;
unsigned short int gen = 1;
unsigned short int colonne, righe;


void input(std::vector<std::vector<bool>> &board) {

    // dimeniosns
    if (IsKeyPressed(KEY_UP) && righe > 1) righe--;
    if (IsKeyPressed(KEY_DOWN)) righe++;
    if (IsKeyPressed(KEY_LEFT) && colonne > 1) colonne--;
    if (IsKeyPressed(KEY_RIGHT)) colonne++;

    // cells state
    int y = GetMouseY() / cellsize.y;
    int x = GetMouseX() / cellsize.x;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) board[y][x] = 1;
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) board[y][x] = 0;

    // save file
    if (IsKeyPressed(KEY_ENTER)) {
        writeMBM(std::to_string(gen), board);
        gen++;
    }
}

void update(std::vector<std::vector<bool>> &board) {

    // aggiorna celle
    board.resize(righe);

    for (size_t i = 0; i < board.size(); i++) {
        board[i].resize(colonne);
    }

    // aggiorna grandezza
    cellsize.y = GetScreenHeight() / righe;
    cellsize.x = GetScreenWidth() / colonne;
}

void draw(std::vector<std::vector<bool>> &board) {

    BeginDrawing();
    ClearBackground(BLACK);

    for (int y = 0; y < board.size(); y++) {
        for (int x = 0; x < board[y].size(); x++) {

            Vector2 pos = { cellsize.x * x, cellsize.y * y };

            Color cell_color = (board[y][x]) ? WHITE : BLACK;
            Color grid_color = (board[y][x]) ? BLACK : WHITE;

            DrawRectangleV(pos, cellsize, cell_color);
            DrawRectangleLines(pos.x, pos.y, cellsize.x, cellsize.y, grid_color);
        }
    }

    DrawText(TextFormat("map: %d", gen), 80, 100, 50, RED);

    EndDrawing();
}


int main() {

    InitWindow(0, 0, "simulazione");
    SetWindowState(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(30);

    std::vector<std::vector<bool>> board;
    plsDontFail(board);

    while (!WindowShouldClose()) {
        input(board);
        update(board);
        draw(board);
    }
    CloseWindow();
}
