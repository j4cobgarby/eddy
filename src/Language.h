#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <string>
#include <iostream>
#include <map>
#include <vector>

class Language {
public:
  Language(string, vector<string>, map<string, map<int, int>>);
  // vector of file extensions for the language, e.g.
  // py, py3
  vector<string> file_extensions;

  // a map of regex strings and a map of each capture group's attributes
  // atrributes can be something as simple as COLOR_PAIR(4),
  // or can combine attributes like
  // COLOR_PAIR(5) | A_BOLD | A_BLINK
  // although it's probably not a good idea to use the A_BLINK attribute,
  // since many terminals don't support it, and it's distracting
  map<string, map<int, int>> regexes;

  // display name for the language e.g. Python
  string lang_name;
};

#endif
