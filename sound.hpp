#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

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
        filename = find_file(name);
    }

    string find_file(string name)
    {
        // TODO: Cache sounds.  Preload, even.
        vector<string> path_preference{
            "", // cwd
            "/usr/share/booths/",
        };

        for (string prefix : path_preference)
        {
            string filename = prefix + config.sounds[name];
            struct stat buf;
            if (stat(filename.c_str(), &buf) == 0) {
                return filename;
            }
        }
        cerr << "Failed to find soundfile " << config.sounds[name];
    }

    void play_file()
    {
        // Silently fail if nothing was loaded.
        if (filename.empty()) return;

        // Fork and play.
        if (0 == fork())
        {
            // Exec in child process.
            char* buf = new char[1024];
            snprintf(buf, 1024, config.sound_cmd.c_str(), filename.c_str());
            system(buf);
            delete[](buf);
            exit(0);
        }
    }
};
