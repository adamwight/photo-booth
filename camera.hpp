#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "locale.hpp"

using namespace cv;

/**
 * Interface with the capture source
 */
class Camera
{
    VideoCapture cam;

public:
    /**
     * Open the camera device
     */
    void open(int device_id)
    {
        cam.open(device_id);
        //cam.set(CV_CAP_PROP_FPS, 2);
        //cam.set(CV_CAP_PROP_POS_AVI_RATIO, 1);
        //cam.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
        //cam.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	    if (!cam.isOpened())
        {
            std::cerr << _("Cannot open camera") << std::endl;
        }
    }

    /**
     * Capture a frame
     */
    void operator>>(Mat& img)
    {
        cam >> img;
    }

    /**
     * Get the capture dimensions
     */
    Size getSize()
    {
        Size s(
            cam.get(CV_CAP_PROP_FRAME_WIDTH),
            cam.get(CV_CAP_PROP_FRAME_HEIGHT)
        );
        return s;
    }
};

