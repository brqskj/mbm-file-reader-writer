/* 
    NOTE:
    - la mappa DEVE essere una amtrice di 2 VLA booleani
    - fornire SOLO il nome del file da leggere, NON l'estrensione
    - l'estensione del file da leggre DEVE esser .mbm
    - imposta DEBUG_MODE a 1 per stampare le informazioni di debug

    COME USARE:
        #include "carica_mbm.h"

        int main() { 
            std::vector <std::vector <bool>> muri;
            std::string nome_file = "prova";
            const bool DEBUG_MODE = false;
            caricaMBM(nome_file, muri, DEBUG_MODe);
        }

    CREARE UNA MAPPA:
    - crea una mappa che sia:
        - retangolare
        - a celle quadrate uguali
        - celle booleane (o muro o no)
    - scrivi ogni cella in una singola stringa come valore booleano
    - dividi la stringa in 8
        - se all'ultimo byte mancano numeri aggiungici degli 0
    - traduci ogni byte bnario in HEX
    - i primi due caratteri del file
        - numero di colonne (in HEX)
        - numero di crighe (in HEX)
    - e poi tutti gli hex che hai preso dalla mappa in se
    - esempio:  ES E4 FC 63 FE

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

#ifndef CARICACAMPOMBM_H
#define CARICACAMPOMBM_H



#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>


bool DEBUG = 0;

struct Dim {
    size_t x;
    size_t y;
};
Dim dim = {0, 0};


void salvaContenuto(std::string nome_file, std::vector<bool>& stringa) {
    
    std::ifstream file(nome_file, std::ios::binary);
    if (!file) return;


    dim = {0, 0};
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

void organizzaCampo(std::vector<bool>& stringa,std::vector <std::vector <bool>>& muri) {

    muri.resize(dim.y);

    for (size_t i = 0; i < muri.size(); i++) {
        muri[i].resize(dim.x);
    }


    for (size_t i = 0; i < stringa.size(); i++) {

        if (i >= dim.y * dim.x) break;

        size_t y = i / dim.x; 
        size_t x = i % dim.x;

        muri[y][x] = stringa[i];
    }
}

void stampaLog(std::string nome_file,std::vector <bool> stringa, std::vector <std::vector <bool>> muri) {

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

        for (size_t x = 0; x < muri[0].size(); x++) {
            std::cout << muri[y][x];
        }
    }
}

void caricaMBM(std::string nome_file, std::vector <std::vector <bool>>& muri, const bool DEBUG_MODE) { 

    DEBUG = DEBUG_MODE;
    nome_file.append(".mbm");

    std::vector <bool> stringa;
    salvaContenuto(nome_file, stringa);
    organizzaCampo(stringa, muri);

    if (DEBUG) stampaLog(nome_file, stringa, muri);
}



#endif