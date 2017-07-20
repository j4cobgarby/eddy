#include "Editor.h"

void Editor::initLangs() {
  map<string, string> python3 {
    {"number", R"([+-]?(\d*[.])?\d+)"},
    {"string", R"(\"((\\\"|[^\"\n])*)\")"},
    {"keyword", "False|None|True|and|as|asset|break|class|continue|def|del|elif|else|except|finally|for|from|global|if|in|import|in|is|is not|lambda|nonlocal|not|not in|or|pass|raise|return|try|while|with|yield"},
    {"operators", R"(\+|-|\*|\/|%|\*\*|\/\/|==|!=|>=|<=|<<|>>|>|<|=|\+=|-=|\*=|\/=|%=|\*\*=|\/\/=|&|\||\^|~|@)"}
  };
  this->langs["python3"] = python3;
}
