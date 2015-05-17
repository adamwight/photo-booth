#include <cairo/cairo.h>
#include <ext/alloc_traits.h>
#include <opencv2/core/core_c.h>
#include <opencv2/core/types_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <string>
#include <vector>

#include "locale.hpp"

using namespace cv;

class Drawing
{
public:
    static void blank(Mat img)
    {
        img = Scalar(0);
    }

    //displayOverlay(string(display.name), string(buf), 1000);
    //XXX got lazy and not cloning backstore cos we are using cam anyway
    static void drawNum(Mat& img, int num)
    {
        char buf[256];
        snprintf(buf, 256, "%d", num);
        string text = buf;
        int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
        double fontScale = 3;
        int thickness = 3;
        int baseline = 0;
        Size textSize = getTextSize(text, fontFace,
                                fontScale, thickness, &baseline);
        Point textOrg((img.cols - textSize.width)/2,
                      (img.rows + textSize.height)/2);
        putText(img, text, textOrg, fontFace, fontScale,
                Scalar(0, 0, 255), thickness, 8);
    }

    static void spinner(Mat& img, float progress)
    {
        ellipse(img,
            Point(img.cols / 2, img.rows / 2),
            Size(50, 50), 0,
            270, 270 - 360 * progress,
            Scalar(255, 255, 255), -1
        );
    }

    //XXX assumes 4-up
    static void tile(Mat& composite, vector<Mat> snaps)
    {
        composite.create(snaps[0].rows, snaps[0].cols, CV_8UC3);
        composite = Scalar(0);
        float hw = composite.cols / 2;
        float hh = composite.rows / 2;

        const Point2f src[] = {
            Point2f(0, 0),
            Point2f(0, 2 * hh),
            Point2f(2 * hw, 2 * hh)
        };
        const Point2f dst[] = {
            Point2f(0, 0),
            Point2f(0, hh),
            Point2f(hw, hh)
        };
        Mat xform = getAffineTransform(src, dst);
        Size dsize(hw, hh);
        Rect roi(0, 0, hw, hh);
        Mat sub = composite(roi);
        warpAffine(snaps[0], sub, xform, dsize);

        xform = getAffineTransform(src, dst);
        roi = Rect(hw, 0, hw, hh);
        sub = composite(roi);
        warpAffine(snaps[1], sub, xform, dsize);

        xform = getAffineTransform(src, dst);
        roi = Rect(0, hh, hw, hh);
        sub = composite(roi);
        warpAffine(snaps[2], sub, xform, dsize);

        xform = getAffineTransform(src, dst);
        roi = Rect(hw, hh, hw, hh);
        sub = composite(roi);
        warpAffine(snaps[3], sub, xform, dsize);
    }

    static void thermometer(Mat& img, float progress)
    {
        int w = 240;
        int h = 15;

        Point topleft(40, 30), pt1, pt2;
        pt1.x = topleft.x + 2;
        pt1.y = topleft.y + 2;
        pt2.x = pt1.x + (w - 4) * progress;
        pt2.y = pt1.y + h - 4;
        rectangle(img, pt1, pt2, CV_RGB(255,0,0), -1);

        pt2.x = topleft.x + w;
        pt2.y = topleft.y + h;
        rectangle(img, topleft, pt2, CV_RGB(255,255,255), 2);

        int fontFace = FONT_HERSHEY_PLAIN;
        double fontScale = 2;
        int thickness = 2;
        string message;
        if (progress < .1) {
            message = _("Idle");
        } else if (progress < .5) {
            message = _("Make a move.");
        } else {
            message = _("Keep moving!");
        }

        Drawing::text(
            img,
            Point(pt1.x + 10, pt1.y + h + 30),
            message
        );
    }

    /**
     * Adapted from http://stackoverflow.com/questions/11917124/opencv-how-to-use-other-font-than-hershey-with-cvputtext-like-arial#answer-26307882
     */
    static void text(Mat& img, Point2d location, string text)
    {
        // Create Cairo
        cairo_surface_t* surface =
            cairo_image_surface_create(
                CAIRO_FORMAT_ARGB32,
                img.cols,
                img.rows);

        cairo_t* cairo = cairo_create(surface);

        // Wrap Cairo with a Mat
        cv::Mat cairoTarget(
                    cairo_image_surface_get_height(surface),
                    cairo_image_surface_get_width(surface),
                    CV_8UC4,
                    cairo_image_surface_get_data(surface),
                    cairo_image_surface_get_stride(surface));

        // Put image onto Cairo
        cv::cvtColor(img, cairoTarget, cv::COLOR_BGR2BGRA);

        // Set font and write text
        cairo_select_font_face (
            cairo,
            "monospace",
            CAIRO_FONT_SLANT_NORMAL,
            CAIRO_FONT_WEIGHT_NORMAL
        );

        cairo_set_font_size(cairo, 18);
        cairo_set_source_rgb(cairo, 1.0, 1.0, 1.0);

        cairo_move_to(cairo, location.x, location.y);
        cairo_show_text(cairo, text.c_str());

        // Copy the data to the output image
        cv::cvtColor(cairoTarget, img, cv::COLOR_BGRA2BGR);

        cairo_destroy(cairo);
        cairo_surface_destroy(surface);
    }
};
