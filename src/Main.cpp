#include <ncurses.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Editor.h"
#include "Widgets.h"
#include "Language.h"

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

    use_default_colors();

    // make colours pairs
    // they can be reference like
    // COLOR_PAIR(index)
    // the first parameter of each init
    // is the index
    init_pair(1, COLOR_BLACK, COLOR_WHITE); // black on white
    init_pair(2, COLOR_WHITE, COLOR_BLUE); // insert
    init_pair(3, COLOR_BLUE, COLOR_WHITE); // normal

    // The window for the title of eddy, at the top
    title_win = newwin(1, COLS, 0, 0);
    // set colour of window - white bg
    wbkgd(title_win, COLOR_PAIR(1));
    // add a title
    wprintw(title_win, "\teddy v1.2");
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

    // Splash dialog

    showInfoDialog({
        "Hello! Welcome to eddy v0.4b!",
        "",
        "Keep in mind it's in beta, so",
        "you should expect issues. If",
        "you do encounter any, it would",
        "really help to report them on",
        "the Github repository."
    }, 36);
}

int main(int argc, char* argv[])
{
    // The editor. This handles input and stuff
    Editor ed;

    map<string, map<int, int>> testmap {
      {"(test|foo|bar)", {{0, 0}}}
    };
    Language testlang ("test", vector<string>{"test", "t"}, testmap);
    ed.langs.push_back(testlang);

    // local filename, to be set as the actual filename later
    string fn = "";

    // if there's at least one cmd line arg given, take it and use it as the
    // filename
    if(argc > 1)
    {
        fn = argv[1];
        ed = Editor(fn);
    }
    // otherwise, initialise an untitled file for the editor
    else
    {
        ed = Editor();
    }

    // call the function to make windows, etc.
    curses_init();

    // loop while the mode isn't 'x', since if it's 'x', the user wants to exit
    while(ed.getMode() != 'x')
    {
        ed.updateStatus();
        ed.printStatusLine(stat_win);
        ed.printBuff(editor_win);

        wrefresh(editor_win);
        wrefresh(stat_win);
        wrefresh(title_win);

        //redrawwin(stat_win);
        wclear(editor_win);
        //redrawwin(editor_win);

        int input = getch();
        ed.handleInput(input);
    }

    // refresh and exit once the mode is 'x'
    refresh();
    endwin();
    return 0;
}
