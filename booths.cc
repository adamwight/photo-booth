#include <errno.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <time.h>
#include <vector>

#include "camera.hpp"
#include "config.hpp"
#include "locale.hpp"
#include "screen.hpp"
#include "motion_detector.hpp"
#include "drawing.hpp"
#include "timer.hpp"
#include "sound.hpp"

using namespace cv;
using namespace std;

class BoothApp
{
    Camera cam;
    Config config;
    MotionDetector detector;
    Screen display;

public:
    BoothApp()
    {
        // Set locale from the environment.
        Locale::init();

        // Load config settings.
        config.load();

        // Open the camera.
        cam.open(config.camera_id);

        // Set display and motion detector size to match.
        display.setSize(cam.getSize());
        detector.setSize(cam.getSize());

        int success = mkdir(config.output_dir.c_str(), 0755);
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

            // Pause between frames, to let the box cool down.
            int keyCode = waitKey(config.motion_interval * 1000);

            if (keyCode == 'q') {
                exit(0);
            }

            if (detector.isMotion())
            {
                Sound::play("begin");

                vector<Mat> snaps(4);
                for (int i = 0; i < 4; i++)
                {
                    if (i == 0)
                    {
                        countdown(config.initial_countdown);
                    }
                    else
                    {
                        countdown(config.countdown);
                    }

                    Sound::play("shot");
                    Mat captured;
                    cam >> captured;
                    display << captured;

                    // Save to disk, or throw an error.
                    archive(captured, to_string(i));

                    // Stash for building 4-up.
                    captured.copyTo(snaps[i]);

                    waitKey(config.frame_pause * 1000);
                }

                // Build 4-up.
                Mat composite;
                Drawing::tile(composite, snaps);

                // Provide satisfying 10s glimpse of own selves.
                display << composite;
                waitKey(config.satisfied_pause * 1000);

                detector.reset();
            }
        }
    }

    /**
     * Save each frame to a file.
     */
    void archive(Mat img, string suffix = "")
    {
        time_t clocks;
        time(&clocks);
        const struct tm* now = localtime(&clocks);
        char stamp[256];
        strftime(stamp, 256, "%F_%H:%M:%S", now);
        char path[1024];
        if (suffix != "")
        {
            suffix = string("_") + suffix;
        }
        snprintf(path, 1024, "%s/%s%s.jpg", config.output_dir.c_str(), stamp, suffix.c_str());

        struct stat buf;
        if (stat(path, &buf) == 0)
        {
            throw runtime_error(string("This file already exists! [") + path + "]");
        }
        bool success = imwrite(string(path), img);
        if (!success)
        {
            throw runtime_error(string("Could not write image file [") + path + "]");
        }
    }

    /**
     * Run a countdown spinner, overlaid on live camera capture
     */
    void countdown(int count, int end = 0)
    {
        Timer t;
        int remaining, current = 0;

        do {
            remaining = count - t.seconds();
            if (remaining != current)
            {
                Sound::play("tick");
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
    // Create the app and run its eventloop.
    BoothApp().eventloop();
}
