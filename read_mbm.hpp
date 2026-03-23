/*
HOW TO USE:
    #include "read_mbm.hpp"

    int main() {
        std::vector <std::vector <bool>> board;
        readMBM("file name", board);
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

#ifndef READMBM_H
#define READMBM_H



#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>


struct Data {
    std::string name;
    unsigned short int columns;
    unsigned short int rows;
    std::vector<bool> array;

    Data(std::string filename) {
        name = filename + ".mbm";
        columns = 0;
        rows = 0;
    }
};


void copyData(Data &data) {

    std::ifstream file(data.name, std::ios::binary);
    if (!file) return;


    uint8_t byte;
    file.read(reinterpret_cast<char *>(&byte), 1);
    data.columns = byte;

    file.read(reinterpret_cast<char *>(&byte), 1);
    data.rows = byte;

    while (file.read(reinterpret_cast<char *>(&byte), 1)) {
        for (int i = 7; i >= 0; i--) {
            data.array.push_back((byte >> i) & 1);
        }
    }


    file.close();
}

void storeData(std::vector<std::vector<bool>> &board, Data &data) {

    board.resize(data.rows);

    for (size_t i = 0; i < board.size(); i++) {
        board[i].resize(data.columns);
    }


    for (size_t i = 0; i < data.array.size(); i++) {

        if (i >= data.rows * data.columns) break;

        size_t y = i / data.columns;
        size_t x = i % data.rows;

        board[y][x] = data.array[i];
    }
}

void readMBM(std::string filename, std::vector<std::vector<bool>> &board) {
    Data data(filename);
    copyData(data);
    storeData(board, data);
}

void readMBMLog(std::string filename, std::vector<std::vector<bool>> &board) {

    Data data(filename);
    copyData(data);
    storeData(board, data);


    std::cout << "\nfile name: " << data.name << "\nn columns: " << data.columns << "\nn rows: " << data.rows;


    std::cout << "\nraw bytes: ";

    for (size_t i = 0; i < data.array.size(); i++) {
        if (i % 8 == 0) std::cout << "\n\t";
        std::cout << data.array[i];
    }


    std::cout << "\nresulting map: ";

    for (size_t y = 0; y < board.size(); y++) {
        std::cout << "\n\t";

        for (size_t x = 0; x < board[0].size(); x++) {
            std::cout << board[y][x];
        }
    }
}



#endif
