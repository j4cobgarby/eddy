#include "Editor.h"
#include "Widgets.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Editor::Editor() {
    x = 0; y = 0;
    mode = 'n';
    status = "Normal Mode";
    filename = "untitled";
    isnewfile = true;

    buff = new Buffer();
    buff->appendLine("");
}

Editor::Editor(string fn) {
    x = 0; y = 0;
    mode = 'n';
    status = "Normal Mode";
    filename = fn;
    isnewfile = false;

    buff = new Buffer();

    ifstream infile(fn.c_str());
    if(infile.is_open()) {
        while(!infile.eof()) {
            string temp;
            getline(infile, temp);
            buff->appendLine(temp);
        }
    }
    else {
        cerr << "Can't open '" << fn << "'.\n";
        buff->appendLine("");
    }
}

void Editor::updateStatus() {
    status = "jEdit :: ";
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
    status += "\tCOL " + tos(x) + "\tLINE " + tos(y) + "\tEDITING: " + filename;
}

string Editor::tos(int i) {
    stringstream ss;
    ss << i;
    return ss.str();
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
                    "Type the new name below!"
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
                x = buff->lines[y-1].length();
                buff->lines[y-1] += buff->lines[y];
                deleteLine();
                moveUp();
            }
            else {
                buff->lines[y].erase(--x, 1);
            }
            break;
        case KEY_DC:
            if(x == buff->lines[y].length() && y != buff->lines.size() - 1) {
                buff->lines[y] += buff->lines[y+1];
                deleteLine(y+1);
            }
            else {
                buff->lines[y].erase(x, 1);
            }
            break;
        case KEY_ENTER:
        case 10:
            if (y > LINES - 4) return;
            if(x < buff->lines[y].length()) {
                buff->insertLine(buff->lines[y].substr(x, buff->lines[y].length() - x), y + 1);
                buff->lines[y].erase(x, buff->lines[y].length() - x);
            }
            else {
                buff->insertLine("", y+1);
            }
            x = 0;
            moveDown();
            break;
        case KEY_BTAB:
        case KEY_CTAB:
        case KEY_STAB:
        case KEY_CATAB:
        case 9:
            buff->lines[y].insert(x, 4, ' ');
            x += 4;
            break;
        default:
            buff->lines[y].insert(x, 1, char(c));
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
    if(x+1 < COLS && x+1 <= buff->lines[y].length()) {
        x++;
        move(y, x);
    }
}

void Editor::moveUp() {
    if(y-1 >= 0)
        y--;
    if(x >= buff->lines[y].length())
        x = buff->lines[y].length();
    move(y, x);
}

void Editor::moveDown() {
    if(y+1 < LINES && y+1 < buff->lines.size())
        y++;
    if(x >= buff->lines[y].length())
        x = buff->lines[y].length();
    move(y, x);
}

void Editor::printBuff(WINDOW * win) {
    for(int i=0; i<LINES-1; i++) {
        if(i >= buff->lines.size()) {
            move(i, 0);
            wclrtoeol(win);
        }
        else {
            mvwprintw(win, i, 0, buff->lines[i].c_str());
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
