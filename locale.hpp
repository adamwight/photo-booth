#ifndef LOCALE_HPP
#define LOCALE_HPP

#include <iostream>
#include <libintl.h>

using namespace std;

#define _(String) gettext (String)

class Locale {
public:
    static void init() {
        const char* locale = setlocale(LC_ALL, "");
        if (!locale) {
            cerr << "Could not honor locale settings!" << endl;
        }
        bindtextdomain("booths", NULL);
        textdomain("booths");
    }
};

#endif
