#include <ncurses.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Editor.h"

using namespace std;

WINDOW * title;

void curses_init()
{
    initscr();
    noecho();
    raw();
    keypad(stdscr, true);
    title = newwin(1, COLS, 0, 0);
    wprintw(title, "jedit alpha");
    wrefresh(title);
}

int main(int argc, char* argv[])
{
    Editor ed;
    string fn = "";

    if(argc > 1)
    {
        fn = argv[1];
        ed = Editor(fn);
    }
    else
    {
        ed = Editor();
    }

    curses_init();

    while(ed.getMode() != 'x')
    {
        ed.updateStatus();
        ed.printStatusLine();
        ed.printBuff();
        redrawwin(title);
        wrefresh(title);
        int input = getch();
        ed.handleInput(input);
    }


    refresh();
    endwin();
    return 0;
}
