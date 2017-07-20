#ifndef BUFFER_H
#define BUFFER_H

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

    // Convert each tab to 4 spaces
    string remTabs(string);
    // Return a newline seperated string representation of the buffer
    string toString();
};

#endif
