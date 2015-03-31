#include <string.h>
#include <stdlib.h>

class Sound
{
    Config config;

public:
    Sound(const char* name) :
        config("config.yaml")
    {
        string filename = config.sounds[name];

        if (0 == fork())
        {
            char* buf = new char[1024];
            snprintf(buf, 1024, "mpg321 -q %s", filename.c_str());
            system(buf);
            delete(buf);
            exit(0);
        }
    }
};
