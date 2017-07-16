#include "Widgets.h"

void showDialog(string title, vector<string> body, int w) {
    string input;
    int h = 6 + body.size();

    WINDOW * dia = newwin(h, w, (LINES / 2) - (h / 2), (COLS / 2) - (w / 2));
    mvwprintw(dia, h-2, 2, "[ENTER] to continue");
    wborder(dia, 0, 0, 0, 0, 0, 0, 0, 0);

    mvwprintw(dia, 1, (w - title.length()) / 2, title.c_str());
    //mvwprintw(dia, 3, 2, body.c_str());
    int i = 0;
    for (auto const& ln: body) { i++;
        mvwprintw(dia, 2+i, 3, ln.c_str());
    }

    wrefresh(dia);
    redrawwin(dia);
    int d_c;
    while (((d_c = getch()) != KEY_ENTER) && d_c != 10) {
    }
    delwin(dia);
    return;
}

string getDialogInput(string title, vector<string> body, int w) {
    int max = w - 8;
    int h = 9 + body.size();
    string input;

    WINDOW * dia = newwin(h, w, (LINES / 2) - (h / 2), (COLS / 2) - (w / 2));
    mvwprintw(dia, h-2, 2, "[ENTER] to submit");
    wborder(dia, 0, 0, 0, 0, 0, 0, 0, 0);

    mvwprintw(dia, 1, 2, title.c_str());
    int i = 0;
    for (auto const& ln: body) { i++;
        mvwprintw(dia, 3+i, 3, ln.c_str());
    }
    wattron(dia, A_REVERSE);
    mvwprintw(dia, 5 + body.size(), 3, string(max+2, ' ').c_str());
    wattroff(dia, A_REVERSE);

    wrefresh(dia);
    redrawwin(dia);
    int d_c;
    while (((d_c = getch()) != KEY_ENTER) && d_c != 10) {
        if ((d_c == KEY_BACKSPACE || d_c == 127) && input.length() > 0) {
            input.pop_back();
        } else if ((d_c >= 32 && d_c <= 126) && input.length() + 1 <= max) {
            input += d_c;
        }
        wattron(dia, A_REVERSE);
        mvwprintw(dia, 5 + body.size(), 3, string(max+2, ' ').c_str());
        mvwprintw(dia, 5 + body.size(), 4, input.c_str());
        wattroff(dia, A_REVERSE);
        wrefresh(dia);
        redrawwin(dia);
    }
    delwin(dia);
    return input;
}
