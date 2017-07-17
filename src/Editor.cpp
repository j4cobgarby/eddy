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
    status = "Normal Mode";
    filename = "untitled";
    isnewfile = true;

    buff = new Buffer();
    buff->appendLine("");
}

Editor::Editor(string fn) {
    x = 0; y = 0;
    scrolly = 0;
    scrollx = 0;
    mode = 'n';
    status = "Normal Mode";
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
        cerr << "The file you specified doesn't exist: '" << fn << "'.\n";
        buff->appendLine("");
    }
}

void Editor::updateStatus() {
    status = "";
    switch(mode) {
    case 'n':
        status += "<NORMAL>";
        break;
    case 'i':
        status += "<INSERT>";
        break;
    case 'x':
        status += "<EXITING...>";
        break;
    }
    status += "\t" + to_string(y + 1 + scrolly) + ":" + to_string(x + 1) + "\t" + filename;
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
        }
        break;
    case 'i':
        switch(c) {
        case 27:
            mode = 'n';
            break;
        case 127:
        case KEY_BACKSPACE:
            if (x == 0 && y == 0) break;
            if (x == 0 && y > 0) {
                x = buff->lines[y+scrolly-1].length();
                buff->lines[y+scrolly-1] += buff->lines[y+scrolly];
                deleteLine();
                moveUp();
            }
            else {
                buff->lines[y+scrolly].erase(--x, 1);
            }
            break;
        case KEY_DC:
            if(x == buff->lines[y+scrolly].length() && y+scrolly != buff->lines.size() - 1) {
                buff->lines[y+scrolly] += buff->lines[y+scrolly+1];
                deleteLine(y+scrolly+1);
            }
            else {
                buff->lines[y+scrolly].erase(x, 1);
            }
            break;
        case KEY_ENTER:
        case 10:
            if (y > LINES - 4) return;
            if(x < buff->lines[y+scrolly].length()) {
                buff->insertLine(buff->lines[y+scrolly].substr(x, buff->lines[y+scrolly].length() - x), y + scrolly + 1);
                buff->lines[y+scrolly].erase(x, buff->lines[y+scrolly].length() - x);
            }
            else {
                buff->insertLine("", y+scrolly+1);
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
            break;
        default:
            buff->lines[y+scrolly].insert(x, 1, char(c));
            x++;
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
    if (y > LINES - 4) {
        scrollDown();
        return;
    }
    if(y+scrolly+1 < LINES && y+scrolly+1 < buff->lines.size())
        y++;
    if(x >= buff->lines[y].length())
        x = buff->lines[y].length();
    move(y, x);
}

void Editor::printBuff(WINDOW * win) {
    // Iterate lines of the editor
    for(int i=0; i<LINES-2; i++) {
        if(i >= buff->lines.size()) {
            move(i, 0);
            wclrtoeol(win);
        }
        else {
            try {
                mvwprintw(win, i, 0, (buff->lines.at(i + scrolly)).c_str());
            } catch (const out_of_range &oor) {
                // this is expected
            }
        }
        wclrtoeol(win);
    }
    move(y+1, x);
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
