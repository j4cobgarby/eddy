#include <ncurses.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Editor.h"
#include "Widgets.h"

using namespace std;

void escdelay_dialog();

WINDOW * title_win;
WINDOW * editor_win;
WINDOW * stat_win;

void curses_init()
{
    // init ncurses screen, making main window too
    initscr();
    // change how terminal input works
    // initialise colours
    start_color();
    // don't echo typed characters back
    noecho();
    // characters read 1 by 1
    cbreak();
    // get all keys
    keypad(stdscr, true);

    // make colours pairs
    // they can be reference like
    // COLOR_PAIR(index)
    // the first parameter of each init
    // is the index
    init_pair(1, COLOR_BLACK, COLOR_WHITE); // black on white
    init_pair(2, COLOR_WHITE, COLOR_BLUE); // insert
    init_pair(3, COLOR_BLUE, COLOR_WHITE); // normal

    // The window for the title of jedit, at the top
    title_win = newwin(1, COLS, 0, 0);
    // set colour of window - white bg
    wbkgd(title_win, COLOR_PAIR(1));
    // add a title
    wprintw(title_win, "jedit\t");
    // display title
    wrefresh(title_win);

    // init the window for the actual text editing
    editor_win = newwin(LINES - 2, COLS, 1, 0);
    // the window for the status, at the bottom e.g. filename, mode
    stat_win = newwin(1, COLS, LINES-1, 0);

    // redraw both windows
    redrawwin(editor_win);
    redrawwin(title_win);

    // background colour of stat_win
    wbkgd(stat_win, COLOR_PAIR(1));

    // update text
    refresh();

    // get value of environment variable escdelay
    // this program works better if it's '0'
    string escdelay = getenv("ESCDELAY");

    // if ESCDELAY isn't 0, make a dialog box telling the user
    // to make it 0. however, they don't have to if they don't
    // want to.
    if (escdelay != "0") {
        showDialog("Warning",
            {
                "Make sure you have your environment variable",
                "ESCDELAY set to 0, for the best experience",
                "with jedit.",
                "To set it, type (in your shell):",
                "",
                "\texport ESCDELAY=0"
            }
            , 50);
    }
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
