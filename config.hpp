#include <opencv2/opencv.hpp>

using namespace std;

class Config
{
public:
    float motion_interval;
    string output_dir;
    float frame_spacing;
    float satisfied_pause;
    //string soundfile[];
    //string mpg_play;
    int initial_countdown;
    int countdown;
    float minimum_area, max_allowed_area, acculum_weight, motion_score_threshold;

    Config(string path)
    {
        FileStorage fs(path, FileStorage::READ);

        if (!fs.isOpened())
        {
            throw runtime_error(string("Couldn't open config file [") + path + "]");
        }

        fs["motion_interval"] >> motion_interval;
        fs["frame_spacing"] >> frame_spacing;
        fs["satisfied_pause"] >> satisfied_pause;
        fs["output_dir"] >> output_dir;
        fs["countdown"] >> countdown;
        fs["initial_countdown"] >> initial_countdown;
        fs["minimum_area"] >> minimum_area;
        fs["max_allowed_area"] >> max_allowed_area;
        fs["acculum_weight"] >> acculum_weight;
        fs["motion_score_threshold"] >> motion_score_threshold;
    }
};
