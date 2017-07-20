#include "Buffer.h"

Buffer::Buffer() {}

string Buffer::remTabs(string line) {
    int tab = line.find("\t");
    if(tab == line.npos)
        return line;
    else
        return remTabs(line.replace(tab, 1, "    "));
}

string Buffer::toString() {
  string result;
  for (string &line : lines) {
      result += line;
      result += "\n";
  }
  // Note the result has a trailing newline
  return result;
}

int Buffer::yIndexFromIndexInString(int index, string s) {
    string before_index = s.substr(0, index);
    int yindex = 0;
    for (int i = 0; i < before_index.length(); i++) {
      if (before_index[i] == '\n') yindex++;
    }
    return yindex;
}

int Buffer::xIndexFromIndexInString(int index, string s) {
    string before_index = s.substr(0, index);
    size_t last_newline = before_index.find_last_of("\n");
    return before_index.length() - last_newline;
}

void Buffer::insertLine(string line, int n) {
    line = remTabs(line);
    lines.insert(lines.begin()+n, line);
}

void Buffer::appendLine(string line) {
    line = remTabs(line);
    lines.push_back(line);
}

void Buffer::removeLine(int n) {
    lines.erase(lines.begin()+n);
}
