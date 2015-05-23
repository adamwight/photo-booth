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
* apt-get install git libopencv-dev cmake gettext libcairo2-dev sox libopencv-core2.3 libopencv-imgproc2.3
* Go to the root directory of this project, and run "make".

Credits
=======
Source code by Adam Roses Wight <adamw@ludd.net>, CC-SA-NC.

Debian packaging help from Paul Wise <pabs@debian.org>.

This application is bundled with the following works from freesound,
* S: [klaxon3](http://www.freesound.org/people/ryansnook/sounds/104291/) by ryansnook -- License: CC-SA-NC
* S: [Clock Tick](http://www.freesound.org/people/Jagadamba/sounds/254316/) by Jagadamba -- License: CC-SA-NC
* S: [Transporter beam](http://www.freesound.org/people/Owdeo/sounds/116505/) by Owdeo -- License: CC-SA-NC
