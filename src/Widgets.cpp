#include "Widgets.h"

void showDialog(string title, string body, int h, int w) {
    int max = 40;
    string input;

    WINDOW * dia = newwin(h, w, (LINES / 2) - (h / 2), (COLS / 2) - (w / 2));
    mvwprintw(dia, h-2, 2, "[ENTER]\t[ESC]");
    wborder(dia, 0, 0, 0, 0, 0, 0, 0, 0);

    mvwprintw(dia, 1, 2, title.c_str());
    mvwprintw(dia, 3, 2, body.c_str());

    wrefresh(dia);
    redrawwin(dia);
    int d_c;
    while (((d_c = getch()) != KEY_ENTER) && d_c != 10) {
    }
    delwin(dia);
    return;
}

string getDialogInput(string title, string prompt, int h, int w) {
    int max = 40;
    string input;

    WINDOW * dia = newwin(h, w, (LINES / 2) - (h / 2), (COLS / 2) - (w / 2));
    mvwprintw(dia, h-2, 2, "[ENTER]\t[ESC]");
    wborder(dia, 0, 0, 0, 0, 0, 0, 0, 0);

    mvwprintw(dia, 1, 2, title.c_str());
    mvwprintw(dia, 3, 2, prompt.c_str());

    wrefresh(dia);
    redrawwin(dia);
    int d_c;
    while (((d_c = getch()) != KEY_ENTER) && d_c != 10) {
        if ((d_c == KEY_BACKSPACE || d_c == 127) && input.length() > 0) {
            input.pop_back();
        } else if ((d_c >= 32 && d_c <= 126) && input.length() + 1 <= max) { // 32 -> 126
            input += d_c;
        }
        mvwprintw(dia, 6, 3, string(max, ' ').c_str());
        mvwprintw(dia, 6, 3, input.c_str());
        wrefresh(dia);
        redrawwin(dia);
    }
    delwin(dia);
    return input;
}
