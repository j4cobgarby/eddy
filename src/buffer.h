#pragma once

#include <string>
#include <vector>

using namespace std;

class Buffer
{
public:
    Buffer();

    vector<string> lines;

    void insertLine(string, int);
    void appendLine(string);
    void removeLine(int);

    string remTabs(string);
    string toString();
};
