#include "editor.h"

using namespace std;

istream& Editor::safeGetline(istream& is, string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a streambuf.
    // That is faster than reading tm (idk what the fuck tm means, i was testing out eddytion and accidentaly erased the original word) one-by-one using the istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    istream::sentry se(is, true);
    streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case EOF:
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}
