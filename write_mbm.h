/*
EXAMPLE HOW TO USE:

    #include "write_mbm.h"
    #include <vector>

    int main() {
        std::vector<std::vector<bool>> board;
        // edit map here
        saveMBM("file name", board);
    }
*/
/*
NOTES:
    - the map MUST be  std::vector <std::vector <bool>>
    - you MUST edit the content of the map or it'll fail
    - id recommand using the graphic funcions to edi the map
*/
/*
HOW DOES THIS WORK?
    - it creates a file with a generation name
    - writes the dimensions
    - writes the data
*/


#ifndef WRITEMBM_H
#define WRITEMBM_H



#include <bitset>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>


extern unsigned short int colonne, righe;


// its not working??
// try this to see if yu screwd up or i forgot sum
void plsDontFail(std::vector<std::vector<bool>> &board) {
    colonne = 8;
    righe = 8;
    board.assign(righe, std::vector<bool>(colonne, false));
}

void fill(std::ofstream &file, std::vector<std::vector<bool>> &board) {

    // resize
    righe = board.size();
    colonne = board[0].size();

    file.write(reinterpret_cast<char *>(&colonne), 1);
    file.write(reinterpret_cast<char *>(&righe), 1);


    // dill w data
    uint8_t byte = 0;
    size_t i = 0;

    for (int y = 0; y < righe; y++) {
        for (int x = 0; x < colonne; x++) {

            if (board[y][x]) byte |= (1 << (7 - i));
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

void saveMBM(std::string FILE_NAME, std::vector<std::vector<bool>> &board) {

    std::string filename = FILE_NAME + ".mbm";
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return;


    fill(file, board);


    file.close();
}



#endif
