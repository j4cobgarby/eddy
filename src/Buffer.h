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

    /* Some helper functions */
    void insertLine(string, int);
    void appendLine(string);
    void removeLine(int);

    /* Substitutes all tabs in string for 4 spaces, so that
     * the tabs won't screw everything up */
    string remTabs(string);
};

#endif
