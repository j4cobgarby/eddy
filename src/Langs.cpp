#include "Editor.h"

void Editor::initLangs() {
  this->langs["python3"] = map<string, string>{
    {"function", R"((\w+)\()"},
    {"operator", R"((\+|-|\*|\/|%|\*\*|\/\/|==|!=|>=|<=|<<|>>|>|<|=|\+=|-=|\*=|\/=|%=|\*\*=|\/\/=|&|\||\^|~|@))"},
    {"keyword", R"((?:\b|\d)(continue|nonlocal|finally|assert|except|global|import|lambda|return|False|break|class|raise|while|yield|None|True|elif|else|from|pass|with|and|def|del|for|not|try|as|if|in|is|or)(?:\b|\d))"},
    {"number", R"(([+-]?(\d*[.])?\d+))"},
    {"string", R"((\"((\\\"|[^\"\n])*)\"))"},
  };
  file_extension_to_lang["python3"] = vector<string>{
    "py", "py3"
  };


  // new languages here
  // ...


  // Now set the currentLang based on the file extension
  set_current_lang();
}

void Editor::set_current_lang() {
  for (pair<string, vector<string>> lang_def : file_extension_to_lang) {

    for (string file_ext : lang_def.second) {
        if (get_file_extension(filename) == file_ext) {
            currentLang = lang_def.first;
        }
    }
  }
}
