#include <vector>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Screen
{
    Mat backstore;

public:
    const char* name;
    Screen(Size size) :
        name("ui"),
        backstore(Mat::zeros(size, CV_8UC3))
    {
        namedWindow(name, CV_WINDOW_NORMAL);
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    }

    Mat getBackstore()
    {
        return backstore;
    }

    void refresh()
    {
        imshow(name, backstore);
    }

    void refresh(const Mat& img)
    {
        backstore = img.clone();
        refresh();
    }

    void operator<<(const Mat& img)
    {
        refresh(img);
    }
};
