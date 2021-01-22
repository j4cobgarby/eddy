#include "Widgets.h"

bool showConfirmDialog(string title, vector<string> body, int width) {
    string input;

    // Calculate height based on
    // top and bottom border = 2
    // + title = 3
    // + text at bottom = 4
    // + blank spaces = 6
    // + amount of lines in body
    int height = 6 + body.size();

    // Make window in center of screen
    WINDOW * dia = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
    // Write message at bottom

    // Yes      No

    wattron(dia, A_BOLD);
    mvwaddstr(dia, height-2, 4, "[y]");
    wattroff(dia, A_BOLD);

    wprintw(dia, "es");
    wprintw(dia, string(5, ' ').c_str());

    wattron(dia, A_BOLD);
    waddstr(dia, "[n]");
    wattroff(dia, A_BOLD);

    wprintw(dia, "o");
    // default border
    wborder(dia, 0, 0, 0, 0, 0, 0, 0, 0);

    // print title in center
    wattron(dia, A_BOLD|A_UNDERLINE);
    mvwprintw(dia, 1, (width - title.length()) / 2, title.c_str());
    wattroff(dia, A_BOLD|A_UNDERLINE);

    int i = 0;
    for (auto const& ln: body) { i++;
        mvwprintw(dia, 2 + i, 3, ln.c_str());
    }

    wrefresh(dia);
    redrawwin(dia);
    int d_c;
    while (true)
    {
        switch(d_c = getch())
        {
            case 'N':
            case 'n':
                return false;
                break;
            case 'y':
            case 'Y':
                return true;
                break;
        }
    }
    delwin(dia);
}

void showInfoDialog(vector<string> body, int width) {
    string input;

    // Calculate height based on
    // top and bottom border = 2
    // + title = 3
    // + text at bottom = 4
    // + blank spaces = 6
    // + amount of lines in body
    int height = 6 + body.size();

    // Make window in center of screen
    WINDOW * dia = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
    // Write message at bottom

    mvwaddstr(dia, height-2, 4, "Press ");

    wattron(dia, A_BOLD);
    waddstr(dia, "[Any key]");
    wattroff(dia, A_BOLD);

    waddstr(dia, " to continue");

    wborder(dia, 0, 0, 0, 0, 0, 0, 0, 0);

    // print title in center
    wattron(dia, A_BOLD|A_UNDERLINE);
    mvwprintw(dia, 1, 2, "Info");
    wattroff(dia, A_BOLD|A_UNDERLINE);

    int i = 0;
    for (auto const& ln: body) { i++;
        mvwprintw(dia, 2 + i, 3, ln.c_str());
    }

    wrefresh(dia);
    redrawwin(dia);
    int d_c;
    d_c = getch();
    delwin(dia);
    return;
}

string getDialogInput(string title, vector<string> body, int width) {
    /*
     * max: max characters in input
     * input_width: visual width of input field
     * height: height of dialog window
     * scroll_amount: amount scrolled in input field, horizontally
     * input: the typed text for the input field
     */
    int max = 128;
    int input_width = width - 6;
    int height = 8 + body.size();
    int scroll_amount = 0;

    string input;

    // Center the window
    WINDOW * dia = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
    mvwprintw(dia, height-2, 2, "[ENTER] to submit");
    // 0 is the default border
    wborder(dia, 0, 0, 0, 0, 0, 0, 0, 0);

    // print title string
    wattron(dia, A_BOLD|A_UNDERLINE);
    mvwprintw(dia, 1, (width - title.length()) / 2, title.c_str());
    wattroff(dia, A_BOLD|A_UNDERLINE);

    // iterate body vector and print each line in it
    int i = 0;
    for (auto const& ln: body) { i++;
        mvwprintw(dia, 2 + i, 3, ln.c_str());
    }
    // initial display of field
    wattron(dia, A_REVERSE);
    mvwprintw(dia, 4 + body.size(), 3, string(input_width, ' ').c_str());
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

            // auto scroll left
            if (input.length() < scroll_amount+1) scroll_amount -= 4;
        }
        // If the pressed character is an allowed ascii character, and the
        // length limit won't be exceeded
        else if ((d_c >= 32 && d_c <= 126) && input.length() + 1 <= max) {
            input += d_c;

            // auto scroll right
            if (input.length() - scroll_amount > input_width) scroll_amount += 5;
        }

        // scroll left if left pressed
        else if (d_c == KEY_LEFT && scroll_amount > 0) {
            scroll_amount--;
        }

        // scroll right if right pressed
        else if (d_c == KEY_RIGHT && scroll_amount < input.length()) {
            scroll_amount++;
        }

        // Black on white
        wattron(dia, A_REVERSE);

        // Draw input box
        mvwprintw(dia, 4 + body.size(), 3, string(input_width, ' ').c_str());

        if (scroll_amount > input.length()) scroll_amount = input.length();
        if (scroll_amount < 0) scroll_amount = 0;

        // Draw text buffer
        mvwprintw(dia, 4 + body.size(), 3, input.substr(scroll_amount, input_width).c_str());

        // White on black
        wattroff(dia, A_REVERSE);

        wrefresh(dia);
        redrawwin(dia);
    }
    delwin(dia);
    return input;
}

vector<string> getFindReplaceFields() {
    string find_input = "";
    string replace_input = "";
    int selected_field = 0; // 0=find, 1=replace, other=replace
    int max = 256; // max length of each input field
    int scroll_amount = 0;

    int height = 11;
    int width = 30;
    int input_width = width - 6;
    string title = "Find and Replace";

    /*

    This is a rather complicated function.
    Basically, it needs to keep track of two different input fields
    so it has an integer selected_field denoting which field is
    selected, and then when the user types something they can
    change the selected field with tab, or type into it.
    The text is but in a temporary buffer copied from the selected
    field, and then afterwards copied back into the field.

    */

    // Make window in center of screen
    WINDOW * dia = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
    // Write message at bottom
    mvwprintw(dia, height - 2, 2, "[ENTER] to go");
    // default border
    wborder(dia, 0, 0, 0, 0, 0, 0, 0, 0);

    // print title in center
    wattron(dia, A_BOLD|A_UNDERLINE);
    mvwprintw(dia, 1, (width - title.length()) / 2, title.c_str());
    wattroff(dia, A_BOLD|A_UNDERLINE);

    mvwprintw(dia, 3, 3, "Find (regex)");
    mvwprintw(dia, 6, 3, "Replace");

    wattron(dia, A_REVERSE);
    mvwprintw(dia, 4, 3, string(input_width, ' ').c_str());
    mvwprintw(dia, 7, 3, string(input_width, ' ').c_str());
    wattroff(dia, A_REVERSE);
    mvwprintw(dia, (selected_field == 0 ? 4 : 7), 2, ">");

    wrefresh(dia);
    redrawwin(dia);
    int d_c;
    while (((d_c = getch()) != KEY_ENTER) && d_c != 10) {
        string current_input = (selected_field == 0 ? find_input : replace_input);
        int current_input_y = (selected_field == 0 ? 4 : 7);

        // If backspace is pressed, and backspacing won't go past the beginning
        // of the input
        if ((d_c == KEY_BACKSPACE || d_c == 127) && current_input.length() > 0) {
            current_input.pop_back();

            // auto scroll left
            if (current_input.length() < scroll_amount+1) scroll_amount -= 4;
        }
        // If the pressed character is an allowed ascii character, and the
        // length limit won't be exceeded
        else if ((d_c >= 32 && d_c <= 126) && current_input.length() + 1 <= max) {
            current_input += d_c;

            // auto scroll right
            if (current_input.length() - scroll_amount > input_width) scroll_amount += 5;
        }

        // scroll left if left pressed
        else if (d_c == KEY_LEFT && scroll_amount > 0) {
            scroll_amount--;
        }

        // scroll right if right pressed
        else if (d_c == KEY_RIGHT && scroll_amount < current_input.length()) {
            scroll_amount++;
        }

        if (selected_field == 0) {
            find_input = current_input;
        } else {
            replace_input = current_input;
        }

        if (d_c == KEY_BTAB
            || d_c == KEY_CTAB
            || d_c == KEY_STAB
            || d_c == KEY_CATAB
            || d_c == 9) {

            selected_field = (selected_field == 0 ? 1 : 0);
        }

        mvwprintw(dia, 4, 2, " ");
        mvwprintw(dia, 7, 2, " ");
        mvwprintw(dia, (selected_field == 0 ? 4 : 7), 2, ">");

        // Black on white
        wattron(dia, A_REVERSE);

        // Draw input box
        mvwprintw(dia, current_input_y, 3, string(input_width, ' ').c_str());

        if (scroll_amount > current_input.length()) scroll_amount = current_input.length();
        if (scroll_amount < 0) scroll_amount = 0;

        // Draw text buffer
        mvwprintw(dia, current_input_y, 3, current_input.substr(scroll_amount, input_width).c_str());

        // White on black
        wattroff(dia, A_REVERSE);

        wrefresh(dia);
        redrawwin(dia);
    }
    delwin(dia);

    vector<string> result {find_input, replace_input};
    return result;
}

vector<string> getOpenURLFields() {
    string url_input = "";
    string local_input = "";
    int selected_field = 0; // 0=find, 1=replace, other=replace
    int max = 256; // max length of each input field
    int scroll_amount = 0;

    int height = 11;
    int width = 30;
    int input_width = width - 6;
    string title = "Open URL";

    /*

    This is a rather complicated function.
    Basically, it needs to keep track of two different input fields
    so it has an integer selected_field denoting which field is
    selected, and then when the user types something they can
    change the selected field with tab, or type into it.
    The text is but in a temporary buffer copied from the selected
    field, and then afterwards copied back into the field.

    */

    // Make window in center of screen
    WINDOW * dia = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
    // Write message at bottom
    mvwprintw(dia, height - 2, 2, "[ENTER] to go");
    // default border
    wborder(dia, 0, 0, 0, 0, 0, 0, 0, 0);

    // print title in center
    mvwprintw(dia, 1, (width - title.length()) / 2, title.c_str());

    mvwprintw(dia, 3, 3, "URL");
    mvwprintw(dia, 6, 3, "Local path");

    wattron(dia, A_REVERSE);
    mvwprintw(dia, 4, 3, string(input_width, ' ').c_str());
    mvwprintw(dia, 7, 3, string(input_width, ' ').c_str());
    wattroff(dia, A_REVERSE);
    mvwprintw(dia, (selected_field == 0 ? 4 : 7), 2, ">");

    wrefresh(dia);
    redrawwin(dia);
    int d_c;
    while (((d_c = getch()) != KEY_ENTER) && d_c != 10) {
        string current_input = (selected_field == 0 ? url_input : local_input);
        int current_input_y = (selected_field == 0 ? 4 : 7);

        // If backspace is pressed, and backspacing won't go past the beginning
        // of the input
        if ((d_c == KEY_BACKSPACE || d_c == 127) && current_input.length() > 0) {
            current_input.pop_back();

            // auto scroll left
            if (current_input.length() < scroll_amount+1) scroll_amount -= 4;
        }
        // If the pressed character is an allowed ascii character, and the
        // length limit won't be exceeded
        else if ((d_c >= 32 && d_c <= 126) && current_input.length() + 1 <= max) {
            current_input += d_c;

            // auto scroll right
            if (current_input.length() - scroll_amount > input_width) scroll_amount += 5;
        }

        // scroll left if left pressed
        else if (d_c == KEY_LEFT && scroll_amount > 0) {
            scroll_amount--;
        }

        // scroll right if right pressed
        else if (d_c == KEY_RIGHT && scroll_amount < current_input.length()) {
            scroll_amount++;
        }

        if (selected_field == 0) {
            url_input = current_input;
        } else {
            local_input = current_input;
        }

        if (d_c == KEY_BTAB
            || d_c == KEY_CTAB
            || d_c == KEY_STAB
            || d_c == KEY_CATAB
            || d_c == 9) {

            selected_field = (selected_field == 0 ? 1 : 0);
        }

        mvwprintw(dia, 4, 2, " ");
        mvwprintw(dia, 7, 2, " ");
        mvwprintw(dia, (selected_field == 0 ? 4 : 7), 2, ">");

        // Black on white
        wattron(dia, A_REVERSE);

        // Draw input box
        mvwprintw(dia, current_input_y, 3, string(input_width, ' ').c_str());

        if (scroll_amount > current_input.length()) scroll_amount = current_input.length();
        if (scroll_amount < 0) scroll_amount = 0;

        // Draw text buffer
        mvwprintw(dia, current_input_y, 3, current_input.substr(scroll_amount, input_width).c_str());

        // White on black
        wattroff(dia, A_REVERSE);

        wrefresh(dia);
        redrawwin(dia);
    }
    delwin(dia);

    vector<string> result {url_input, local_input};
    return result;
}
