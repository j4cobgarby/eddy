#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <string>
#include <map>
#include <iostream>
#include <vector>

using namespace std;

class Language {
public:
  Language(string, vector<string>, map<string, map<int, int>>);

  string lang_name;
  vector<string> file_extensions;

  /*
  a map containing regexes and maps which store the attributes
  of each capture group of the regex
  */
  map<string, map<int, int>> regexes;
};

#endif
