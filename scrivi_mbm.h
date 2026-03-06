/*
    NOTE:
    - è consigliato usare lo script "graifca pre-save"

    COME USARE:
        #include "scrivi_mbm.h"
        int main() {
            Campo campo;
            campo.salva();  //campo.avvia_grafica();
        }

    COME FUNZIONA .mbm :
    - i primi 2 byte sono numeri INT tradotti in binario
        - il primo è il numero di bit x riga (o colonne)
        - il secondo è il numero di righe
    - tutti gli altry byte rappresentano la mappa
        - 1 = muro, 0 = no muro
        - sono una stringa unica divisa in byte per comodità
        - 1 byte non è per forza una riga
    - puoi aprirli con
        - HEX visualizer (devi tradurre in binario)
        - blocco note generico (UTF-8)(non vedrai molto)
*/

#ifndef SALVAGRAFICA_H
#define SALVAGRAFICA_H



#include "raylib.h"
#include <bitset>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>


unsigned short int gen = 1;


class Campo {
public:
    int colonne, righe;
    Vector2 size;
    std::vector<std::vector<bool>> celle;

    Campo() {
        this->colonne = 8;
        this->righe = 8;
    }

    //          SALVATAGGIO
    // ----------------------------------
    void riempi(std::ofstream &file) {

        // dimensioni
        file.write(reinterpret_cast<char *>(&colonne), 1);
        file.write(reinterpret_cast<char *>(&righe), 1);

        // dati
        uint8_t byte = 0;
        size_t i = 0;

        for (int y = 0; y < righe; y++) {
            for (int x = 0; x < colonne; x++) {

                if (celle[y][x]) byte |= (1 << (7 - i));
                i++;

                if (i == 8) {
                    file.write(reinterpret_cast<char *>(&byte), 1);
                    byte = 0;
                    i = 0;
                }
            }
        }

        if (i > 0) file.write(reinterpret_cast<char *>(&byte), 1);
    }

    void salva() {

        // crea
        std::string filename = std::to_string(gen) + ".mbm";
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) return;


        riempi(file);


        file.close();
        gen++;
    }

    //          ENGINE GRAFICO
    // ----------------------------------
    void input() {

        // dimensione
        if (IsKeyPressed(KEY_UP) && righe > 1) righe--;
        if (IsKeyPressed(KEY_DOWN)) righe++;
        if (IsKeyPressed(KEY_LEFT) && colonne > 1) colonne--;
        if (IsKeyPressed(KEY_RIGHT)) colonne++;

        // stato celle
        int y = GetMouseY() / size.y;
        int x = GetMouseX() / size.x;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) celle[y][x] = 1;
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) celle[y][x] = 0;
    }

    void aggiorna() {

        // aggiorna celle
        celle.resize(righe);

        for (size_t i = 0; i < celle.size(); i++) {
            celle[i].resize(colonne);
            //
        }

        // aggiorna grandezza
        size.x = GetScreenWidth() / colonne;
        size.y = GetScreenHeight() / righe;
    }

    void disegna() {

        BeginDrawing();
        ClearBackground(BLACK);

        for (int y = 0; y < celle.size(); y++) {
            for (int x = 0; x < celle[y].size(); x++) {

                Color colore = (celle[y][x]) ? WHITE : BLACK;
                Vector2 pos = { size.x * x, size.y * y };
                DrawRectangleV(pos, size, colore);
                DrawRectangleLines(pos.x, pos.y, size.x, size.y, WHITE);
            }
        }

        DrawText(TextFormat("map: %d", gen), 80, 100, 50, RED);

        EndDrawing();
    }

    void avvia_grafica() {

        InitWindow(0, 0, "simulazione");
        SetTargetFPS(30);

        while (!WindowShouldClose()) {
            input();
            aggiorna();
            disegna();
            if (IsKeyPressed(KEY_ENTER)) salva();
        }

        CloseWindow();
    }
};


#endif
