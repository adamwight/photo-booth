#include <vector>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Screen
{
    Mat backstore;
    const char* name;

public:
    Screen() :
        name("ui")
    {
        namedWindow(name, CV_WINDOW_NORMAL);
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    }

    void setSize(Size size)
    {
        backstore = Mat::zeros(size, CV_8UC3);
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
