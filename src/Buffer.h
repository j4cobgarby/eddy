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
    string toString(int, int);
    // Get's the actual y index of something in a file, based on it's
    // index in a given string and the amount of newlines before it.
    int yIndexFromIndexInString(int, string);
    // Get's the to-display x index.
    // this is calculated by getting a substring of the buffer string
    // from 0 to the index of the token
    // then, finding the index of the last newline character in this
    // and then subtracting that newline's index from the length of
    // the substring
    int xIndexFromIndexInString(int, string);
};

#endif
