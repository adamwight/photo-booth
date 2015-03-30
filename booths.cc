#include <errno.h>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include <opencv2/opencv.hpp>

#include "camera.hpp"
#include "screen.hpp"
#include "motion_detector.hpp"
#include "drawing.hpp"
#include "timer.hpp"
#include "sound.hpp"

using namespace cv;
using namespace std;

class BoothApp
{
    Camera& cam;
    Screen& display;
    MotionDetector detector;

public:
    BoothApp(Camera& cam, Screen& display) :
        cam(cam),
        display(display),
        detector(cam.getSize())
    {
        // FIXME: hardcoded output dir
        int success = mkdir("archive", 0755);
        if (success != 0 && errno != EEXIST)
        {
            throw runtime_error("Couldn't create output directory! [");
        }
    }

    void eventloop()
    {
        for (;;)
        {
            // Capture a frame.
            Mat snap;
            cam >> snap;
            // Feed it to the motion detector.
            detector.addFrame(snap);
            // Overlay a score for the amount of motion detected.
            Drawing::thermometer(snap, (float)detector.score / (float)detector.motion_score_threshold);
            // Draw to the screen.
            display << snap;

            // Wait 20ms.
            waitKey(20);

            if (detector.isMotion())
            {
                Sound("begin");
                countdown(10, 5);

                vector<Mat> snaps(4);
                for (int i = 0; i < 4; i++)
                {
                    countdown(5);

                    Mat tmp;
                    cam >> tmp;
                    Sound("shot");
                    display << tmp;
                    tmp.copyTo(snaps[i]);
                    waitKey(500);
                }

                // Build 4-up.
                Mat composite;
                Drawing::tile(composite, snaps);

                // Save to disk, or throw an error.
                archive(snaps);

                // Provide satisfying 10s glimpse of own selves.
                display << composite;
                waitKey(10 * 1000);
                detector.reset();
            }
        }
    }

    void archive(vector<Mat> series)
    {
        time_t clocks;
        time(&clocks);
        const struct tm* now = localtime(&clocks);
        for (int i = 0; i < series.size(); i++)
        {
            char stamp[256];
            strftime(stamp, 256, "%F_%H:%M:%S", now);
            char path[256];
            snprintf(path, 256, "archive/%s_%d.jpg", stamp, i);

            bool success = imwrite(string(path), series[i]);
            if (!success)
            {
                throw runtime_error(string("Could not write image file [") + path + "]");
            }
        }
    }

    void countdown(int count, int end = 0)
    {
        Timer t;
        int remaining, current = 0;

        do {
            remaining = count - t.seconds();
            if (remaining != current)
            {
                Sound("tick");
                current = remaining;
            }
            float fractional = 1 - t.fractional();

            Mat flash;
            cam >> flash;
            Drawing::spinner(flash, fractional);
            Drawing::drawNum(flash, remaining);
            display << flash;
            waitKey(1);
        } while (remaining > end);
    }
};

int main()
{
    Camera cam(0);
    Screen display(cam.getSize());

    BoothApp app(cam, display);
    app.eventloop();
}
