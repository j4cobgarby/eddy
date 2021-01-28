#pragma once

#include <ncurses.h>
#include <string>
#include <vector>

using namespace std;

bool             showConfirmDialog    (string, vector<string>, int);
void             showInfoDialog       (vector<string>, int);
string           getDialogInput       (string, vector<string>, int);
vector<string>   getFindReplaceFields ();
vector<string>   getOpenURLFields     ();
