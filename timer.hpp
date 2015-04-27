#include <time.h>
#include <math.h>
#include <sys/time.h>

class Timer
{
    timespec start;

public:
    Timer()
    {
        clock_gettime(CLOCK_REALTIME, &start);
    }

    int seconds()
    {
        return floor(get());
    }

    float fractional()
    {
        return get() - seconds();
    }

    float get()
    {
        timespec now;
        clock_gettime(CLOCK_REALTIME, &now);

        float giga = 1000 * 1000 * 1000;
        float nanodiff = (now.tv_sec - start.tv_sec) * giga + (now.tv_nsec - start.tv_nsec);

        return nanodiff / giga;
    }
};
