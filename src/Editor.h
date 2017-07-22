#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>
#include <regex>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Buffer.h"

class Editor
{
private:
    int     x, y;
    int     scrolly;
    int     scrollx;
    char    mode;
    bool    isnewfile;
    bool    modified;

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
    void    openFile(string);

    void    doFindReplace(string, string);

    int     digits_in_num(int);
    int     longest_line_number(vector<string>);
public:
    Editor();
    Editor(string);

    char getMode() {return mode;}

    void handleInput(int);
    void printBuff(WINDOW * win);
    void printStatusLine(WINDOW * win);
    void updateStatus();

    istream& safeGetline(istream&, string&);
};

#endif
