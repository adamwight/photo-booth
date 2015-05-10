#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <vector>

#include "config.hpp"

using namespace cv;

RNG rng(12345);

class MotionDetector
{
public:
    Mat steady;
    Mat current;
    Mat difference;
    float score, minimum_area, motion_score_threshold, acculum_weight, max_allowed_area;
    bool initialized;
    Config config;

public:
    MotionDetector() :
        initialized(false),
        score(0)
    {
        minimum_area = config.minimum_area;
        max_allowed_area = config.max_allowed_area;
        acculum_weight = config.acculum_weight;
        motion_score_threshold = config.motion_score_threshold;
    }

    void setSize(Size size)
    {
        steady = Mat::zeros(size, CV_32F);
    }

    void addFrame(Mat img)
    {
        cvtColor(img, current, CV_BGR2GRAY);
        current.convertTo(current, CV_32F, 1 / 255.0);
        //blur(current, current, Size(5, 5));

        calculate_diff();

        accumulateWeighted(current, steady, acculum_weight);
    }

    void calculate_diff()
    {
        if (!initialized)
        {
            initialized = true;
            reset();
            return;
        }

        absdiff(steady, current, difference);
        difference.convertTo(difference, CV_8U, 255);

        threshold(difference, difference, 5, 255, CV_THRESH_BINARY);
        //adaptiveThreshold(difference, difference, 255, ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 7, 8);

        int size = 10;
        Mat kernel = getStructuringElement(
            MORPH_ELLIPSE,
            Size(2*size + 1, 2*size + 1),
            Point(size, size)
        );
        //dilate(difference, difference, kernel);
        //erode(difference, difference, kernel);

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(difference, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        //Mat out = Mat::zeros(difference.size(), CV_8UC3);
        //Mat out = current;
        float max_move = 0;
        for (int i = 0; i < contours.size(); i++)
        {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            //drawContours(out, contours, i, color, 2, 8, hierarchy, 0, Point());
            Rect bndRect = boundingRect(contours[i]);
            Point pt1, pt2;
            pt1.x = bndRect.x;
            pt1.y = bndRect.y;
            pt2.x = bndRect.x + bndRect.width;
            pt2.y = bndRect.y + bndRect.height;
            //rectangle(out, pt1, pt2, CV_RGB(255,255,255), 2);

            float area_proportion = (float)(bndRect.width * bndRect.height) / (float)(steady.rows * steady.cols);
            if (area_proportion > max_move && area_proportion < max_allowed_area)
            {
                max_move = area_proportion;
            }
        }
        score -= .05;
        if (max_move > minimum_area)
        {
//std::cerr << "Largest moving area: " << max_move << std::endl;
            score += .3;
        }
        if (score < 0)
        {
            score = 0;
        }

        if (config.debug) {
            cerr << "Motion score = " << score << endl;
        }
    }

    bool isMotion()
    {
        return score > motion_score_threshold;
    }

    void reset()
    {
        score = 0;
        current.copyTo(steady);
    }
};
