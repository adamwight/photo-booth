#ifndef LOCALE_HPP
#define LOCALE_HPP

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <libintl.h>                    // for bindtextdomain, textdomain
#include <locale.h>                     // for setlocale, LC_ALL, NULL

using namespace std;

#define _(String) gettext (String)

class Locale {
public:
    static void init() {
        const char* locale = setlocale(LC_ALL, "");
        if (!locale) {
            cerr << "Could not honor locale settings!" << endl;
        }
        bindtextdomain("photo-booth", NULL);
        textdomain("photo-booth");
    }
};

#endif
