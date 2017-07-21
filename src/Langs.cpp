#include "Editor.h"

void Editor::initLangs() {
  this->langs["python3"] = map<string, string> {
    {"a", R"((\w+)\()"},
    {"b", R"((\+|-|\*|\/|%|\*\*|\/\/|==|!=|>=|<=|<<|>>|>|<|=|\+=|-=|\*=|\/=|%=|\*\*=|\/\/=|&|\||\^|~|@))"},
    {"c", R"((?:\b|\d)(continue|nonlocal|finally|assert|except|global|import|lambda|return|False|break|class|raise|while|yield|None|True|elif|else|from|pass|with|and|def|del|for|not|try|as|if|in|is|or)(?:\b|\d))"},
    {"d", R"(([+-]?(\d*[.])?\d+))"},
    {"e", R"((\"((\\\"|[^\"\n])*)\"))"},
  };
  file_extension_to_lang["python3"] = vector<string>{
    "py", "py3"
  };

  // really, this is c++ and c
  this->langs["c++"] = map<string, string> {
    {"a", R"((\w+)\()"},
    {"b", R"((\+|-|\*|/|%|\+\+|--|==|!=|>|<|>=|<=|&&|\|\||!|&|\||\^|\~|<<|>>|=|\+=|-=|\*=|/=|%=|<<=|>>=|&=|\^=|\|=|\.|->|::))"},
    {"c", R"((?:\b|\d)(reinterpret_cast|atomic_noexcept|atomic_cancel|atomic_commit|conceptconst|dynamic_cast|static_asset|synchronized|thread_local|static_cast|const_cast|constexpr|namespace|protected|char16_t|char32_t|continue|decltype|explicit|noexcept|operator|register|requires|template|typename|unsigned|volatile|alignas|alignof|default|explort|mutable|nullptr|private|typedef|virtual|wchar_t|and_eq|bitand|delete|double|extern|friend|import|inline|module|not_eq|public|return|signed|sizeof|static|struct|switch|typeid|xor_eq|bitor|break|catch|class|compl|false|float|or_eq|short|throw|union|using|while|auto|bool|case|char|else|enum|goto|long|this|true|void|and|asm|for|int|new|not|try|xor|do|if|or)(?:\b|\d))"},
    {"d", R"(([+-]?(\d*[.])?\d+))"},
    {"e", R"((\"((\\\"|[^\"\n])*)\"))"},
  };
  file_extension_to_lang["c++"] = vector<string>{
    "c++", "cc", "h", "cxx", "cpp", "hh", "H", "hxx", "hpp", "h++", "c"
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
