#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>

#include "Buffer.h"

class Editor
{
private:
    int x, y;
    char mode;
    Buffer* buff;
    string status, filename;

    /* For those of you who do not have -std=c++11 in g++ */
    string tos(int);

    // Cursor movement
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void deleteLine();                  // Deletes current line
    void deleteLine(int);               // Deletes line <int>

    void saveFile();                    // Saves buffer into the file

public:
    Editor();                           // Normal constructor
    Editor(string);                     // Constructor accepting filename

    char getMode() {return mode;}

    void handleInput(int);              // Handles keyboard input
    void printBuff();
    void printStatusLine();             // Prints the status line (like vim!!!)
    void updateStatus();                // Updates the status line (text, not display)
};

#endif
