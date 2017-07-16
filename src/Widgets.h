#ifndef WIDGETS_H
#define WIDGETS_H

#include <ncurses.h>
#include <string>
#include <vector>

using namespace std;

void showDialog(string title, vector<string> body, int h, int w);
string getDialogInput(string, string, int, int);

#endif
