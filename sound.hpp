#include <string.h>
#include <stdlib.h>

class Sound
{
public:
    Sound(const char* name)
    {
        if (0 == strcmp(name, "begin"))
        {
            name = "sounds/borg_tractor_beam.mp3";
        } 
        else if (0 == strcmp(name, "tick"))
        {
            name = "sounds/romulan_computerbeep1.mp3";
        }
        else if (0 == strcmp(name, "shot"))
        {
            name = "sounds/tos_keypress3.mp3";
        }

        if (0 == fork())
        {
            char* buf = new char[256];
            snprintf(buf, 256, "mpg321 -q %s", name);
            system(buf);
            delete(buf);
            exit(0);
        }
    }
};
