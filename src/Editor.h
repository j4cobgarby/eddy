#pragma once

#include <ncurses.h>
#include <regex>
#include <fstream>
#include <sstream>

#include "Buffer.h"
#include "Language.h"

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

    void    scrollRight();
    void    scrollLeft();

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
    void setMode(char m) {mode = m;}

    void handleInput(int);
    void printBuff(WINDOW * win);
    void printStatusLine(WINDOW * win);
    void updateStatus();

    istream& safeGetline(istream&, string&);

    vector<Language> langs;
    Language current_lang;
};
