#ifndef WIDGETS_H
#define WIDGETS_H

#include <ncurses.h>
#include <string>
#include <vector>

using namespace std;

void   showDialog     (string, vector<string>, int);
string getDialogInput (string, vector<string>, int);

#endif
