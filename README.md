Self-serve photo booth software.  Flail your arms until the motion
detector is activated, then beam your party into the future!

Installing
==========

* Download and install the Debian package from the
[releases](https://github.com/adamwight/photo-booth/releases) page, or build
the binary according to the instructions in "Development" below.

Configuration
=============

Edit `/etc/photo-booth/config.yaml`.

Running
=======

* photo-booth

Press the "q" key to exit.  By default, images will be saved to ~/photo-booth-archive

Development
===========

* git clone https://github.com/adamwight/photo-booth.git
* Follow this [tutorial](http://docs.opencv.org/doc/tutorials/introduction/linux_install/linux_install.html#linux-installation) to build an OpenCV development environment.
* apt-get install git libopencv-dev cmake gettext libcairo2-dev sox libopencv-core2.\* libopencv-imgproc2.\* git2cl
* Go to the root directory of this project, and run "make".

To build the .deb package,
* git clone https://github.com/adamwight/photo-booth-debian.git debian
* make release

Credits
=======
Source code by Adam Roses Wight <adamw@ludd.net>, GPL-3.

Debian packaging help from Paul Wise <pabs@debian.org>.

This application is bundled with the following works from freesound,
* S: [futuretrocomputing_10_suonho](http://www.freesound.org/people/suonho/sounds/3647/) by suonho, remixed by adamwight using `sox in out remix - trim 0:03` -- License: CC-BY-3.0
* S: [camera_snap1](http://www.freesound.org/people/thecheeseman/sounds/51360/) by thecheeseman -- License: CC-BY-3.0
* S: [btn402](http://www.freesound.org/people/junggle/sounds/26777/) by junggle -- License: CC-BY-3.0
