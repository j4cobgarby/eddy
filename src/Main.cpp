#include <ncurses.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Editor.h"

using namespace std;

WINDOW * title_win;
WINDOW * editor_win;

void curses_init()
{
    initscr();
    noecho();
    raw();
    keypad(stdscr, true);
    title_win = newwin(1, COLS, 0, 0);
    wattron(title_win, A_REVERSE);
    wprintw(title_win, "jedit alpha, written by j4cobgarby");
    wclrtoeol(title_win);
    wrefresh(title_win);

    editor_win = newwin(LINES - 2, COLS, 1, 0);
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
        ed.printBuff(editor_win);
        redrawwin(editor_win);
        wrefresh(editor_win);
        redrawwin(title_win);
        wrefresh(title_win);
        int input = getch();
        ed.handleInput(input);
    }


    refresh();
    endwin();
    return 0;
}
