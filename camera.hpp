#include <opencv2/opencv.hpp>

using namespace cv;

class Camera
{
    VideoCapture cam;
public:
    Camera(int device_id) :
        cam(device_id)
    {
        //cam.set(CV_CAP_PROP_FPS, 2);
        //cam.set(CV_CAP_PROP_POS_AVI_RATIO, 1);
        //cam.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
        //cam.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
    }

    void operator>>(Mat& img)
    {
        cam >> img;
    }

    Size getSize()
    {
        Size s(
            cam.get(CV_CAP_PROP_FRAME_WIDTH),
            cam.get(CV_CAP_PROP_FRAME_HEIGHT)
        );
        return s;
    }
};

