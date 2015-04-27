#ifndef CONFIG_HPP
#define CONFIG_HPP 1

#include <map>
#include <opencv2/core/core.hpp>
#include <opencv2/core/operations.hpp>
#include <string>
#include <stdexcept>
#include <vector>

#include "locale.hpp"

using namespace cv;
using namespace std;

class Config
{
public:
    float motion_interval;
    string output_dir;
    float frame_pause;
    float satisfied_pause;
    int camera_id;
    int initial_countdown;
    int countdown;
    float minimum_area, max_allowed_area, acculum_weight, motion_score_threshold;
    string sound_cmd;
    map<string, string> sounds;

    void load()
    {
        vector<string> path_preference{
            "config.yaml",
            "/etc/booths/config.yaml"
        };
        FileStorage fs;
        for (string &path : path_preference)
        {
            fs.open(path, FileStorage::READ);

            if (fs.isOpened())
            {
                break;
            }
        }
        if (!fs.isOpened())
        {
            // TODO: Log search path.
            throw runtime_error(string(_("Couldn't find config file.")));
        }

        fs["motion_interval"] >> motion_interval;
        fs["frame_pause"] >> frame_pause;
        fs["satisfied_pause"] >> satisfied_pause;
        fs["output_dir"] >> output_dir;
        fs["camera_id"] >> camera_id;
        fs["countdown"] >> countdown;
        fs["initial_countdown"] >> initial_countdown;
        fs["minimum_area"] >> minimum_area;
        fs["max_allowed_area"] >> max_allowed_area;
        fs["acculum_weight"] >> acculum_weight;
        fs["motion_score_threshold"] >> motion_score_threshold;
        fs["sound_cmd"] >> sound_cmd;

        // Copy the map of soundfiles.
        for (FileNode sound : fs["sounds"])
        {
            sounds[sound.name()] = (string)sound;
        }
    }
};

#endif
