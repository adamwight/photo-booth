#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

class Sound
{
    Config config;

public:
    Sound(const char* name)
    {
        // TODO: Cache result.
        vector<string> path_preference{
            "",
            "/usr/share/booths/",
        };

        for (string prefix : path_preference)
        {
            string filename = prefix + config.sounds[name];
            struct stat buf;
            if (stat(filename.c_str(), &buf) == 0) {
                play_file(filename);
                return;
            }
        }
        // TODO: Log failure.
    }

protected:
    void play_file(string filename)
    {
        if (0 == fork())
        {
            char* buf = new char[1024];
            snprintf(buf, 1024, config.sound_cmd.c_str(), filename.c_str());
            system(buf);
            delete(buf);
            exit(0);
        }
    }
};
