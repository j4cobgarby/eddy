#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>
#include <regex>

#include "Buffer.h"

class Editor
{
private:
    int     x, y;
    int     scrolly;
    int     scrollx;
    char    mode;
    bool    isnewfile;

    Buffer* buff;

    string  status, filename;

    void    moveUp();
    void    moveDown();
    void    moveLeft();
    void    moveRight();

    void    scrollDown();
    void    scrollUp();

    void    scrollDown(int);
    void    scrollUp(int);

    void    deleteLine();
    void    deleteLine(int);

    void    saveFile();

    void doFindReplace(string, string);
public:
    Editor();
    Editor(string);

    char getMode() {return mode;}

    void handleInput(int);
    void printBuff(WINDOW * win);
    void printStatusLine(WINDOW * win);
    void updateStatus();
};

#endif
