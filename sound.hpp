#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include "config.hpp"
#include "locale.hpp"

using namespace std;

class Sound
{
    Config config;

public:
    /**
     * Play a sound by name
     */
    static void play(string name)
    {
        // Load a sound and play it.
        Sound(name).play_file();
    }

protected:
    string filename;

    Sound(string name)
    {
        config.load(); // FIXME
        filename = find_file(name);
    }

    string find_file(string name)
    {
        // TODO: Preload and cache sounds.
        // TODO: Path preferences should be built into the install script.
        vector<string> path_preference{
            "./sounds/",
            "/usr/share/photo-booth/sounds/",
        };

        // Try each possible path and play the first match.
        for (string prefix : path_preference)
        {
            string filename = prefix + config.sounds[name];
            struct stat buf;
            // The file exists?
            if (stat(filename.c_str(), &buf) == 0) {
                return filename;
            }
        }
        // We fell through...
        // FIXME: lego string
        cerr << _("Failed to find soundfile ") << config.sounds[name];
        // kludge
        return "";
    }

    void play_file()
    {
        // Silently fail if nothing was loaded.
        if (filename.empty()) return;

        // Fork and play.
        if (0 == fork())
        {
            // In child process, run the commandline in a shell.
            auto buf = new char[1024];
            snprintf(buf, 1024, config.sound_cmd.c_str(), filename.c_str());
            if (config.debug) {
                cout << "Playing sound " << buf << endl;
            }
            int exit_code = system(buf);
            if (exit_code != 0) {
                cerr << "Cannot play sound!" << endl;
            }
            delete[](buf);
            exit(0);
        }
    }
};
