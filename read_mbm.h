/*
EXAMPLE HOW TO USE:

    #include "read_mbm.h"

    int main() {
        std::vector <std::vector <bool>> board;
        bool DEBUG_MODE = false;

        caricaMBM("file_name", board, DEBUG_MODE);
    }
*/
/*
NOTES:
    - the reciving map MUST be  std::vector <std::vector <bool>>
    - give ONLY the file name, NOT the format
    - the file to read MUST be .mbm
*/
/*
HOW DOES THIS WORK?
    - you give the file you want to read and the map you want to fill
    - it saves every bit of the file in a vector
    - it resizes teh amp based on the dimensions
    - it fills the map with the data
*/

#ifndef CARICACAMPOMBM_H
#define CARICACAMPOMBM_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

inline bool DEBUG = 0;

struct Dim {
    size_t x;
    size_t y;
};
inline Dim dim = { 0, 0 };


void salvaContenuto(std::string nome_file, std::vector<bool> &stringa) {

    std::ifstream file(nome_file, std::ios::binary);
    if (!file) return;

    dim = { 0, 0 };
    char byte;
    bool primo = true;
    bool secondo = true;

    while (file.read(&byte, 1)) {
        for (int i = 7; i >= 0; i--) {

            bool bit = (byte >> i) & 1;

            if (primo) dim.x += pow(2, i) * bit;
            else if (secondo) dim.y += pow(2, i) * bit;
            else stringa.push_back(bit);
        }

        if (!primo) secondo = false;
        primo = false;
    }

    file.close();
}

void organizzaCampo(std::vector<bool> &stringa, std::vector<std::vector<bool>> &muri) {

    muri.resize(dim.y);

    for (size_t i = 0; i < muri.size(); i++)
        muri[i].resize(dim.x);

    for (size_t i = 0; i < stringa.size(); i++) {

        if (i >= dim.y * dim.x) break;

        size_t y = i / dim.x;
        size_t x = i % dim.x;

        muri[y][x] = stringa[i];
    }
}

void stampaLog(std::string nome_file, std::vector<bool> stringa, std::vector<std::vector<bool>> muri) {

    std::cout << "\nDEBUG MODE ON \n";

    std::cout << "\nnome file: " << nome_file;

    std::cout << "\nbytes: ";
    for (size_t i = 0; i < stringa.size(); i++) {
        if (i % 8 == 0) std::cout << "\n\t";
        std::cout << stringa[i];
    }

    std::cout << "\nn bit x riga: " << dim.x;
    std::cout << "\nn righe: " << dim.y;

    std::cout << "\nmappa effettiva: ";
    for (size_t y = 0; y < muri.size(); y++) {
        std::cout << "\n\t";

        for (size_t x = 0; x < muri[0].size(); x++)
            std::cout << muri[y][x];
    }
}

void caricaMBM(std::string nome_file, std::vector<std::vector<bool>> &muri, const bool DEBUG_MODE) {

    DEBUG = DEBUG_MODE;
    nome_file.append(".mbm");

    std::vector<bool> stringa;
    salvaContenuto(nome_file, stringa);
    organizzaCampo(stringa, muri);

    if (DEBUG) stampaLog(nome_file, stringa, muri);
}

#endif
