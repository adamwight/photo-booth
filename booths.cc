#include <stdio.h>
#include <iostream>
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
    }

    void eventloop()
    {
        for (;;)
        {
            Mat snap;
            cam >> snap;
            detector.addFrame(snap);
            Drawing::thermometer(snap, (float)detector.score / (float)detector.motion_score_threshold);
            display << snap;
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

                Mat composite;
                Drawing::tile(composite, snaps);

                archive(snaps);

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

            imwrite(string(path), series[i]);
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
