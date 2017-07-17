#include "Widgets.h"

void showDialog(string title, vector<string> body, int w) {
    string input;
    int height = 6 + body.size();

    WINDOW * dia = newwin(height, w, (LINES / 2) - (height / 2), (COLS / 2) - (w / 2));
    mvwprintw(dia, height - 2, 2, "[ENTER] to continue");
    wborder(dia, 0, 0, 0, 0, 0, 0, 0, 0);

    mvwprintw(dia, 1, (w - title.length()) / 2, title.c_str());

    int i = 0;
    for (auto const& ln: body) { i++;
        mvwprintw(dia, 2 + i, 3, ln.c_str());
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
    /*
     * max: max characters in input
     * input_width: visual width of input field
     * height: height of dialog window
     * scroll_amount: amount scrolled in input field, horizontally
     * input: the typed text for the input field
     */
    int max = 128;
    int input_width = w - 6;
    int height = 9 + body.size();
    int scroll_amount = 0;

    string input;

    // Center the window
    WINDOW * dia = newwin(height, w, (LINES / 2) - (height / 2), (COLS / 2) - (w / 2));
    mvwprintw(dia, height-2, 2, "[ENTER] to submit");
    // 0 is the default border
    wborder(dia, 0, 0, 0, 0, 0, 0, 0, 0);

    // print title string
    mvwprintw(dia, 1, 2, title.c_str());

    // iterate body vector and print each line in it
    int i = 0;
    for (auto const& ln: body) { i++;
        mvwprintw(dia, 3 + i, 3, ln.c_str());
    }
    // initial display of field
    wattron(dia, A_REVERSE);
    mvwprintw(dia, 5 + body.size(), 3, string(input_width, ' ').c_str());
    wattroff(dia, A_REVERSE);

    wrefresh(dia);
    redrawwin(dia);
    int d_c;
    // Until enter is pressed, constantly update value of d_c by getting the
    // character pressed
    while (((d_c = getch()) != KEY_ENTER) && d_c != 10) {
        // If backspace is pressed, and backspacing won't go past the beginning
        // of the input
        if ((d_c == KEY_BACKSPACE || d_c == 127) && input.length() > 0) {
            input.pop_back();
        }
        // If the pressed character is an allowed ascii character, and the
        // length limit won't be exceeded
        else if ((d_c >= 32 && d_c <= 126) && input.length() + 1 <= max) {
            input += d_c;
        }

        else if (d_c == KEY_LEFT && scroll_amount > 0) {
            scroll_amount--;
        }

        else if (d_c == KEY_RIGHT && scroll_amount < input.length()) {
            scroll_amount++;
        }

        // Black on white
        wattron(dia, A_REVERSE);

        // Draw input box
        mvwprintw(dia, 5 + body.size(), 3, string(input_width, ' ').c_str());

        if (scroll_amount > input.length()) scroll_amount = input.length();
        if (scroll_amount < 0) scroll_amount = 0;

        // Draw text buffer
        mvwprintw(dia, 5 + body.size(), 3, input.substr(scroll_amount, input_width).c_str());

        // White on black
        wattroff(dia, A_REVERSE);

        wrefresh(dia);
        redrawwin(dia);
    }
    delwin(dia);
    return input;
}
