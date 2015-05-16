#ifndef LOCALE_HPP
#define LOCALE_HPP

#include <iostream>
#include <libintl.h>
#include <locale.h>
//#include <QTextCodec>

using namespace std;

#define _(String) gettext (String)

class Locale {
public:
    static void init() {
        const char* locale = setlocale(LC_ALL, "");
        if (!locale) {
            cerr << "Could not honor locale settings!" << endl;
        }
        bindtextdomain("photo-booth", nullptr);
        textdomain("photo-booth");

#ifdef WITH_QT
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif
    }
};

#endif
