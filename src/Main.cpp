#include <ncurses.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Editor.h"

using namespace std;

void escdelay_dialog();

WINDOW * title_win;
WINDOW * editor_win;
WINDOW * stat_win;

void curses_init()
{
    initscr();
    start_color();
    noecho();
    raw();
    keypad(stdscr, true);

    init_pair(1, COLOR_BLACK, COLOR_WHITE); // black on white
    init_pair(2, COLOR_WHITE, COLOR_BLUE); // insert
    init_pair(3, COLOR_BLUE, COLOR_WHITE); // normal

    title_win = newwin(1, COLS, 0, 0);
    wbkgd(title_win, COLOR_PAIR(1));
    wprintw(title_win, "jedit\t");
    wclrtoeol(title_win);
    wrefresh(title_win);

    editor_win = newwin(LINES - 2, COLS, 1, 0);
    stat_win = newwin(1, COLS, LINES-1, 0);

    redrawwin(editor_win);
    redrawwin(title_win);

    wbkgd(stat_win, COLOR_PAIR(1));

    refresh();
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
        ed.printStatusLine(stat_win);
        ed.printBuff(editor_win);

        wrefresh(editor_win);
        wrefresh(stat_win);
        wrefresh(title_win);

        redrawwin(editor_win);
        redrawwin(title_win);
        redrawwin(stat_win);

        int input = getch();
        ed.handleInput(input);
    }


    refresh();
    endwin();
    return 0;
}
