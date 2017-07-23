#include "Language.h"

Language::Language(string lang_name, vector<string> file_extensions, map<string, map<int, int>> regexes) {
  this->lang_name = lang_name;
  this->file_extensions = file_extensions;
  this->regexes = regexes;
}

Language::Language() {
  lang_name = "text";
}
