#include "Editor.h"
#include "Widgets.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Editor::Editor() {
    x = 0; y = 0;
    scrolly = 0;
    scrollx = 0;
    mode = 'n';
    status = "";
    filename = "untitled";
    isnewfile = true;
    modified = false;

    buff = new Buffer();
    buff->appendLine("");
}

Editor::Editor(string fn) {
    x = 0; y = 0;
    scrolly = 0;
    scrollx = 0;
    mode = 'n';
    status = "";
    filename = fn;
    isnewfile = false;
    modified = false;

    buff = new Buffer();

    fstream infile(fn.c_str());
    if(infile.is_open()) {
        while(!infile.eof()) {
            string temp;
            getline(infile, temp);
            buff->appendLine(temp);
        }
    }
    else {
        cerr << "The file you specified doesn't exist: '" << fn << "'.\n";
        buff->appendLine("");
    }
}

void Editor::updateStatus() {
    status = "";
    switch(mode) {
    case 'n':
        status += "Normal";
        break;
    case 'i':
        status += "Insert";
        break;
    case 'x':
        status += "Exiting";
        break;
    }
    status += "\t" + to_string(y + 1 + scrolly) + ":" + to_string(x + 1) + "\t" + filename;
    if (modified) {
        status += "*";
    }
}

void Editor::handleInput(int c) {
    switch(c) {
    case KEY_LEFT:
        moveLeft();
        return;
    case KEY_RIGHT:
        moveRight();
        return;
    case KEY_UP:
        moveUp();
        return;
    case KEY_DOWN:
        moveDown();
        return;
    }
    switch(mode) {
    case 'n':
        switch(c) {
        case ']':
            scrollDown();
            break;
        case '[':
            scrollUp();
            break;
        case '}':
            scrollDown(10);
            break;
        case '{':
            scrollUp(10);
            break;
        case 'x':
            if (modified) {
                if (isnewfile) {
                    filename = getDialogInput("Save", {
                        "Don't forget to name your file!",
                        "You're currently editing an untitled",
                        "file, which isn't a very good name.",
                        "Type the name (or path) below"
                    }, 54);
                    isnewfile = false;
                }
                saveFile();
            }
            mode = 'x';
            break;
        case 'i':
            mode = 'i';
            break;
        case 's':
            if (isnewfile) {
                filename = getDialogInput("Save", {
                    "Currently, this file is unnamed. To",
                    "save your hard work, you need to give",
                    "it a name!",
                    "Type the name (or path) below"
                }, 54);
                isnewfile = false;
            }
            saveFile();
            break;
        case 'f':
            {
                {
                    vector<string> fields = getFindReplaceFields();
                    string find_field = fields.at(0);
                    string replace_field = fields.at(1);
                    doFindReplace(find_field, replace_field);
                    modified = true;
                    x = 0;
                }
                break;
            }
        case 'o':
            openFile(getDialogInput("Open", {
                "Type a file path"
            }, 40));
            break;
        case 'O':
            {
              vector<string> fields = getOpenURLFields();
              string wget = "wget -O "+fields.at(1)+" "+fields.at(0);
              system(wget.c_str());
              openFile(fields.at(1));
            }
        }
        break;
    case 'i':
        switch(c) {
        case 27:
            mode = 'n';
            break;
        case 127:
        case KEY_BACKSPACE:
            if (x == 0 && y+scrolly == 0) break;
            if (x == 0 && y+scrolly > 0) {
                x = buff->lines[y+scrolly-1].length();
                buff->lines[y+scrolly-1] += buff->lines[y+scrolly];
                deleteLine();
                moveUp();
                modified = true;
            }
            else {
                buff->lines[y+scrolly].erase(--x, 1);
                modified = true;
            }
            break;
        case KEY_DC:
            if(x == buff->lines[y+scrolly].length() && y+scrolly != buff->lines.size() - 1) {
                buff->lines[y+scrolly] += buff->lines[y+scrolly+1];
                deleteLine(y+scrolly+1);
                modified = true;
            }
            else {
                buff->lines[y+scrolly].erase(x, 1);
                modified = true;
            }
            break;
        case KEY_ENTER:
        case 10:
            if(x < buff->lines[y+scrolly].length()) {
                buff->insertLine(buff->lines[y+scrolly].substr(x, buff->lines[y+scrolly].length() - x), y + scrolly + 1);
                buff->lines[y+scrolly].erase(x, buff->lines[y+scrolly].length() - x);
                modified = true;
            }
            else {
                buff->insertLine("", y+scrolly+1);
                modified = true;
            }
            x = 0;
            moveDown();
            break;
        case KEY_BTAB:
        case KEY_CTAB:
        case KEY_STAB:
        case KEY_CATAB:
        case 9:
            buff->lines[y+scrolly].insert(x, 4, ' ');
            x += 4;
            modified = true;
            break;
        case '{':
            buff->lines[y+scrolly].insert(x, "{}");
            x++;
            modified = true;
            break;
        case '(':
            buff->lines[y+scrolly].insert(x, "()");
            x++;
            modified = true;
            break;
        case '[':
            buff->lines[y+scrolly].insert(x, "[]");
            x++;
            modified = true;
            break;
        default:
            buff->lines[y+scrolly].insert(x, 1, char(c));
            x++;
            modified = true;
            break;
        }
        break;
    }
}

void Editor::moveLeft() {
    if(x-1 >= 0) {
        x--;
        move(y, x);
    }
}

void Editor::moveRight() {
    if(x+1 < COLS && x+1 <= buff->lines[y+scrolly].length()) {
        x++;
        move(y, x);
    }
}

void Editor::scrollUp() {
    scrollUp(1);
}

void Editor::scrollDown() {
    scrollDown(1);
}

void Editor::scrollUp(int amount) {
    scrolly -= amount;
    if (scrolly < 0) scrolly = 0;
}

void Editor::scrollDown(int amount) {
    scrolly += amount;
    if (scrolly > (buff->lines.size() - (LINES / 2))) scrolly = buff->lines.size() - (LINES / 2);
}

void Editor::moveUp() {
    if (y < 4 && scrolly > 0) {
        scrollUp();
        return;
    }
    if(y+scrolly-1 >= 0)
        y--;
    if(x >= buff->lines[y].length())
        x = buff->lines[y].length();
    move(y, x);
}

void Editor::moveDown() {
    if (y > LINES-(3 + 4)) {
        scrollDown();
        return;
    }
    if(y+1 < LINES && y+1 < buff->lines.size())
        y++;
    if(x >= buff->lines[y].length())
        x = buff->lines[y].length();
    move(y, x);
}

int Editor::digits_in_num(int n) {
    return to_string(n).length();
}

// longest by digits
int Editor::longest_line_number(vector<string> vec) {
    return digits_in_num(vec.size());
}

void Editor::printBuff(WINDOW * win) {
    int longest_ln_number = longest_line_number(buff->lines);

    // Iterate lines of the editor. Subtract 2 because of the title and status
    // bars
    for(int i=0; i<LINES-2; i++) {
        int col_pair = (mode == 'n' ? COLOR_PAIR(3) : COLOR_PAIR(2));
        wattron(win, col_pair);
        mvwprintw(win, i, 0, string(longest_ln_number+1, ' ').c_str());
        wattroff(win, col_pair);

        if (i + scrolly < buff->lines.size()) {
            try {
                wattron(win, col_pair);
                mvwprintw(win, i, 0, to_string(i + scrolly + 1).c_str());
                wattroff(win, col_pair);
                mvwprintw(win, i, longest_ln_number+2, (buff->lines.at(i + scrolly).substr(0, COLS)).c_str());
            } catch (out_of_range oor) {}
        }
        wclrtoeol(win);
    }
    // Move the cursor to the correct location so that the user knows what
    // they're editing
    move(y+1, x+longest_ln_number+2);

    string buffer_string = buff->toString();
    /*
    Now for syntax highlighting
    The basic idea is this:
     - iterate all the language definition regex for the currently selected
        language.
     - put all of the matches, in a pair with their colour, all in a pair with
        its index, in a vector
     - should get something like:

        vector<pair<int, pair<string, int>>>
        a vector of pairs of ints and pairs of strings and ints

        {
            {0, ["print", COLOR_BLUE]},
            {6, ["\"Hello, world!\"", COLOR_GREEN]},
                etc...
        }
    */

    if (currentLang == "" || currentLang == "text" || currentLang == "plaintext")
        return;

    vector<pair<int, pair<string, int>>> matches;
    for (pair<string, string> type : langs[currentLang]) {

        // type.second is each regex
    }
}

void Editor::printStatusLine(WINDOW * win) {
    if (mode == 'n') {
        wbkgd(win, COLOR_PAIR(3));
    } else if (mode == 'i') {
        wbkgd(win, COLOR_PAIR(2));
    } else {
        wbkgd(win, COLOR_PAIR(1));
    }
    mvwprintw(win, 0, 0, status.c_str());
    wclrtoeol(win);
}

void Editor::deleteLine() {
    buff->removeLine(y);
}

void Editor::deleteLine(int i) {
    buff->removeLine(i);
}

void Editor::saveFile() {
    if (filename == "") {
        filename = "untitled";
    }

    ofstream f(filename.c_str());
    if(f.is_open()) {
        for(int i=0; i<buff->lines.size(); i++) {
            f << buff->lines[i] << endl;
        }
        status = "Saved to " + filename;
    }
    else {
        status = "Cannot open " + filename;
    }
    f.close();
}

void Editor::openFile(string fn) {
    x = 0; y = 0;
    scrolly = 0;
    scrollx = 0;
    filename = fn;
    isnewfile = false;

    buff = new Buffer();

    fstream infile(fn.c_str());
    if(infile.is_open()) {
        while(!infile.eof()) {
            string temp;
            getline(infile, temp);
            buff->appendLine(temp);
        }
    }
    else {
        showDialog("Error", {
            "The file you specified doesn't",
            "exist."
        }, 35);
        cerr << "The file you specified doesn't exist: '" << fn << "'.\n";
        buff->appendLine("");
        isnewfile = true;
        filename = "untitled";
    }
}

void Editor::doFindReplace(string find, string replace) {
    regex reg (find);
    for (int i = 0; i < buff->lines.size(); i++) {
        try {
            string temp = buff->lines.at(i);
            temp = regex_replace(temp, reg, replace);
            buff->lines[i] = temp;
        } catch (out_of_range oor) {
            cout << "Out of range" << endl;
        }
    }
}
