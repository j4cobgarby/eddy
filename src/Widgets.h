#ifndef WIDGETS_H
#define WIDGETS_H

#include <ncurses.h>
#include <string>

using namespace std;

void showDialog(string, string, int, int);
string getDialogInput(string, string, int, int);

#endif
